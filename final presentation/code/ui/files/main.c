#include <gtk/gtk.h>                                                            // For GTK+
#include <string.h>                                                             // For strlen()
#include <stdint.h>



#include <stdio.h>
#include <unistd.h>
#include <string.h> /* for strncpy */

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#define PORT    "3490"      // the port users wil be connecting to
#define BACKLOG 10          // how many pending connection queue will hold
#define MAXDATASIZE 8         // max number of bytes we can get at once
#define SIZE_FILENAME 1024

char* get_filename_from_path(char* fp){

     char filepath[SIZE_FILENAME];

     if (fp[0] != '/')
     {
         strcpy(filepath, "/");
         strcat(filepath, fp);
     }else{
         strcpy(filepath, fp);
     }

     char *filename = (char*)calloc(1, sizeof(SIZE_FILENAME));
     filename = (strrchr(filepath, '/'))+1;

     return filename;

 }

void sigchld_handler(int s)
{
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

// get sockaddr, IPv4 or IPv6
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}



GtkBuilder      *builder;
GObject         *windowMain;
GObject         *progressbar;
GObject         *statusbar;

void fs();

int prepareFileSend();
int prepareReceiveFile();
int prepareActivate();

char* myIP(char *iface, char ip[100])
{
    int fd;
    struct ifreq ifr;

    fd = socket(AF_INET, SOCK_DGRAM, 0);

    /* I want to get an IPv4 IP address */
    ifr.ifr_addr.sa_family = AF_INET;

    /* I want IP address attached to "eth0" */
    strncpy(ifr.ifr_name, iface, IFNAMSIZ-1);

    ioctl(fd, SIOCGIFADDR, &ifr);

    close(fd);

    /* display result */
    sprintf(ip, "%s", inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));
    // printf("%s\n", ip);
}

int main(int argc, char *argv[])
{
    GObject         *menuItemQuit;
    GObject         *btnSendFile;
    GObject         *btnReceiveFile;
    GObject         *btnActivate;
    GObject         *listboxFiles;

    GError          *error;
    GdkScreen       *screen;
    GdkDisplay      *display;
    const gchar     *cssFile;                                                   // CSS file
    const gchar     *gladeFile;                                                 // Glade design file
    GtkCssProvider  *provider;
    gsize           bytesWritten, bytesRead;
    const gchar *context_description="context";
    guint cid;

    char            ip[100];
    char            iface[20];


    error           = 0;
    cssFile         = "style.css";
    gladeFile       = "design.glade";


    gtk_init(&argc, &argv);

 /* Construct a GtkBuilder instance and load UI description */
    builder =  gtk_builder_new();
    gtk_builder_add_from_file(builder, gladeFile, NULL);

 /* Load CSSS */
    provider = gtk_css_provider_new();
    display = gdk_display_get_default();
    screen = gdk_display_get_default_screen(display);
    gtk_style_context_add_provider_for_screen(
                                    screen,
                                    GTK_STYLE_PROVIDER(provider),
                                    GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);   // Load CSS file and covert to UTF-8

    gtk_css_provider_load_from_path (provider,
                                     g_filename_to_utf8(cssFile,
                                                        strlen(cssFile),
                                                        &bytesRead,
                                                        &bytesWritten,
                                                        &error),
                                     NULL);

 /* Construct signal handlers to the constructed widgets */

    windowMain = gtk_builder_get_object(builder, "windowMain");                 // Main window
    g_signal_connect(windowMain, "destroy", G_CALLBACK(gtk_main_quit), NULL);   // Action on close button

    menuItemQuit = gtk_builder_get_object(builder, "imagemenuitemQuit");        // Quit item of menu bar
    g_signal_connect(menuItemQuit, "activate", G_CALLBACK(gtk_main_quit), NULL);// Quit program

    btnSendFile = gtk_builder_get_object(builder, "buttonSendFile");
    g_signal_connect(btnSendFile, "clicked", G_CALLBACK(prepareFileSend), NULL);

    btnReceiveFile= gtk_builder_get_object(builder, "buttonReceiveFile");
    g_signal_connect(btnReceiveFile, "clicked", G_CALLBACK(prepareReceiveFile), NULL);

    btnActivate = gtk_builder_get_object(builder, "buttonActivate");
    g_signal_connect(btnActivate, "clicked", G_CALLBACK(prepareActivate), NULL);

    progressbar = gtk_builder_get_object(builder, "progressbar");
    gtk_widget_hide(GTK_WIDGET(progressbar));







    g_object_unref(provider);
    g_object_unref(windowMain);


    gtk_main();


    return 0;
}

int prepareReceiveFile()
{
        int                     sockfd, new_fd;     // listen on sockd_fd, new connection on new_fd
    struct addrinfo         hints, *servinfo, *p;
    struct sockaddr_storage their_addr;         // connector's address information
    socklen_t               sin_size;
    struct sigaction        sa;
    int                     yes = 1;
    char                    s[INET6_ADDRSTRLEN];
    int                     rv;
    int             numbytes;
    char            buf[MAXDATASIZE];
    FILE                    *file;
    char                    *buffer;                                            // For transferring characters
    ssize_t                 byteSend;                                           // Byte send to receiver
        ssize_t                 byteReceive;
        char    filename[SIZE_FILENAME];
    GObject         *ip;
    char           *recvIp;

    GtkDialog       *dialogMessage;

    GtkMessageType  messageType;

    GtkDialogFlags  flags;

    const gchar            *message;

    const gchar            *messageDescription;

    messageType         = GTK_MESSAGE_INFO;                                     // INFO or ERROR

    message = "File received successfully.";
    messageDescription = "";
        flags       = GTK_DIALOG_DESTROY_WITH_PARENT;


    ip = gtk_builder_get_object(builder, "entryIPAddress");

    recvIp =   (char *) gtk_entry_get_text (GTK_ENTRY(ip));



    buffer          = (char *)malloc(MAXDATASIZE * sizeof(char));


    memset(&hints, 0, sizeof hints);
    hints.ai_family     = AF_UNSPEC;
    hints.ai_socktype   = SOCK_STREAM;
    hints.ai_flags      = AI_PASSIVE;        // use my IP



    if ((rv = getaddrinfo(recvIp, PORT, &hints, &servinfo)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and connect to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next)
    {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
        {
            perror("client: socket");
            continue;
        }

        while (connect(sockfd, p->ai_addr, p->ai_addrlen)
               == -1);
        break;
    }

    if (p == NULL)
    {
        fprintf(stderr, "client: failed to connect\n");
        return 2;
    }

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr), s, sizeof s);
    printf("client: connecting to %s\n", s);

    freeaddrinfo(servinfo);     // all done with this structure

    FILE *fp;
    fp = fopen("tmp", "w+");

    while (*buffer != EOF)
    {
        byteReceive = recv(sockfd, buffer, MAXDATASIZE, 0);
        if (*buffer != EOF)
        {
            fwrite(buffer, 1, sizeof(buffer), fp);
        }


    }

    byteReceive = recv(sockfd, buffer, MAXDATASIZE, 0);

    fclose(fp);
    strcpy(filename, "files/");
    strcat(filename, buffer);
    rename("tmp", filename);


    close(sockfd);

    dialogMessage       = GTK_DIALOG(gtk_message_dialog_new(GTK_WINDOW(windowMain),
                                                 flags,
                                                 messageType,
                                                 GTK_BUTTONS_OK,
                                                 "%s",
                                                 (gchar *)message));
    gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(dialogMessage),
                                             "%s",
                                             (gchar *)messageDescription);

            gtk_dialog_run (GTK_DIALOG(dialogMessage));
            gtk_widget_hide(GTK_WIDGET(dialogMessage));

}

int prepareActivate()
{
    GObject         *iface;
    char           *myIface;
    char            ip[32];
    char            info[32];

    iface = gtk_builder_get_object(builder, "entryIface");
    myIface =   (char *) gtk_entry_get_text (GTK_ENTRY(iface));

    myIP(myIface, info);


    statusbar = gtk_builder_get_object(builder, "statusbar");
    /* get id for the message at the top of the
     * info stack? */
    guint id = gtk_statusbar_get_context_id(GTK_STATUSBAR(statusbar), "info");
    /* show the top message from the info stack
     * ? */
    gtk_statusbar_push(GTK_STATUSBAR(statusbar), id, info);


}

int prepareFileSend()
{
    int                     sockfd, new_fd;     // listen on sockd_fd, new connection on new_fd
    struct addrinfo         hints, *servinfo, *p;
    struct sockaddr_storage their_addr;         // connector's address information
    socklen_t               sin_size;
    struct sigaction        sa;
    int                     yes = 1;
    char                    s[INET6_ADDRSTRLEN];
    int                     rv;
    int             numbytes;
    char            buf[MAXDATASIZE];
    FILE                    *file;
    char                    *buffer;                                            // For transferring characters
    ssize_t                 byteSend;                                           // Byte send to receiver
        ssize_t                 byteReceive;
        char    filename[SIZE_FILENAME];
    gchar     *filePath;


    GObject         *fileChooser;
        GtkDialog       *dialogMessage;

        GtkMessageType  messageType;

        GtkDialogFlags  flags;

        const gchar            *message;

        const gchar            *messageDescription;

        messageType         = GTK_MESSAGE_INFO;                                     // INFO or ERROR

        message = "File sent successfully.";
        messageDescription = "";
            flags       = GTK_DIALOG_DESTROY_WITH_PARENT;





    fileChooser         = gtk_builder_get_object(builder, "filechooserbutton");

    filePath            = (char*)gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(fileChooser));


        buffer          = (char *)malloc(MAXDATASIZE * sizeof(char));

        memset(&hints, 0, sizeof hints);
        hints.ai_family     = AF_UNSPEC;
        hints.ai_socktype   = SOCK_STREAM;
        hints.ai_flags      = AI_PASSIVE;        // use my IP

        if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0)
        {
            fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
            return 1;
        }

        file = fopen(filePath, "r");

        if (file == 0)
        {                                                                           // Could not open file
            perror("file open");
            return 0;
        }

        // loop through all the results and bind to the first we can
        for (p = servinfo; p != NULL; p = p->ai_next)
        {
            if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
            {
                perror("server: socket");
                continue;
            }

            if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
            {
                perror("setsockopt");
                exit(1);
            }

            if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1)
            {
                close(sockfd);
                perror("server: bind");
                continue;
            }

            break;
        }

        if (p == NULL)
        {
            fprintf(stderr, "server: failed to bind\n");
            return 2;
        }

        freeaddrinfo(servinfo);       // all done with this structure

        if (listen(sockfd, BACKLOG) == -1)
        {
            perror("listen");
            exit(1);
        }

        sa.sa_handler = sigchld_handler;    // reap all dead processes
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = SA_RESTART;
        if (sigaction(SIGCHLD, &sa, NULL) == -1)
        {
            perror("sigaction");
        }

        printf("server: waiting for connection...\n");

        // while (1)
        // {
            sin_size = sizeof their_addr;
            new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
            if (new_fd == -1)
            {
                perror("accept");
                // continue;
            }

            inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof s);
            printf("server: got connection from %s\n", s);

            if (!fork())
            {
                // this is the child process
                close(sockfd);      // child doesn't need the listener


                fseek(file, SEEK_SET, 0);
                while (!feof(file))
                {
                    fread(buffer, sizeof(char), MAXDATASIZE, file);
                    byteSend = send(new_fd, buffer, MAXDATASIZE, 0);
                    if (byteSend == -1)
                    {
                        perror("send");
                        return 0;
                    }

                }
                *buffer = EOF;
                byteSend = send(new_fd, buffer, MAXDATASIZE, 0);

                buffer = get_filename_from_path(filePath);
                byteSend = send(new_fd, buffer, MAXDATASIZE, 0);



                close(new_fd);
                exit(0);




                // if (send(new_fd, buff, sizeof(buff), 0) == -1)
                // {
                //     perror("send");
                // }
                close(new_fd);
                exit(0);




            }
            close(new_fd);      // parent doesn't need this
        // }

            dialogMessage       = GTK_DIALOG(gtk_message_dialog_new(GTK_WINDOW(windowMain),
                                                         flags,
                                                         messageType,
                                                         GTK_BUTTONS_OK,
                                                         "%s",
                                                         (gchar *)message));
            gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(dialogMessage),
                                                     "%s",
                                                     (gchar *)messageDescription);

                    gtk_dialog_run (GTK_DIALOG(dialogMessage));
                    gtk_widget_hide(GTK_WIDGET(dialogMessage));

}

// int prepareFileSend2()
// {
//     GObject         *ipAddress;
//     GObject         *fileChooser;
//     GtkDialog       *dialogMessage;
//     GtkMessageType  messageType;
//     GtkDialogFlags  flags;
//     const gchar     *filePath;
//     const gchar     *receiverIPAddress;
//     progressbar = gtk_builder_get_object(builder, "progressbar");
//     gdouble         progressbarFill;
//     gdouble a = 6;
//     gdouble b = 10;


//     const gchar            *message;
//     const gchar            *messageDescription;

//     messageType         = GTK_MESSAGE_INFO;                                     // INFO or ERROR
//     progressbarFill     -= a/b;


//     flags       = GTK_DIALOG_DESTROY_WITH_PARENT;

//     fileChooser         = gtk_builder_get_object(builder, "filechooserbutton");
//     filePath            = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(fileChooser));

//     ipAddress           = gtk_builder_get_object(builder, "entryIPAddress");
//     receiverIPAddress   = gtk_entry_get_text (GTK_ENTRY(ipAddress));

//     message             = receiverIPAddress;
//     messageDescription  = filePath;



//     gtk_widget_show(GTK_WIDGET(progressbar));
//     gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progressbar), (gdouble)(a/b));
//     dialogMessage       = GTK_DIALOG(gtk_message_dialog_new(GTK_WINDOW(windowMain),
//                                                  flags,
//                                                  messageType,
//                                                  GTK_BUTTONS_OK,
//                                                  "%s",
//                                                  (gchar *)message));
//     gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(dialogMessage),
//                                              "%s",
//                                              (gchar *)messageDescription);

//     gtk_dialog_run (GTK_DIALOG(dialogMessage));
//     gtk_widget_hide(GTK_WIDGET(dialogMessage));

//     gtk_widget_hide(GTK_WIDGET(progressbar));

//     statusbar = gtk_builder_get_object(builder, "statusbar");
//     /* get id for the message at the top of the
//      * info stack? */
//     guint id = gtk_statusbar_get_context_id(GTK_STATUSBAR(statusbar), "info");
//     /* show the top message from the info stack
//      * ? */
// gtk_statusbar_pop (GTK_STATUSBAR(statusbar), id);
// }
);
//