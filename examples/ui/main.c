#include <gtk/gtk.h>                                                            // For GTK+
#include <string.h>                                                             // For strlen()
#include <stdint.h>

GtkBuilder      *builder;
GObject         *windowMain;
GObject         *progressbar;
GObject         *statusbar;

void fs();

int16_t prepareFileSend();

int main(int argc, char *argv[])
{
    GObject         *menuItemQuit;
    GObject         *btnSendFile;
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

    progressbar = gtk_builder_get_object(builder, "progressbar");
    gtk_widget_hide(GTK_WIDGET(progressbar));


    statusbar = gtk_builder_get_object(builder, "statusbar");
    /* get id for the message at the top of the
     * info stack? */
    guint id = gtk_statusbar_get_context_id(GTK_STATUSBAR(statusbar), "info");
    /* show the top message from the info stack
     * ? */
    gchar *info = "This was uninitialized";
    gtk_statusbar_push(GTK_STATUSBAR(statusbar), id, info);



    g_object_unref(provider);
    g_object_unref(windowMain);


    gtk_main();


    return 0;
}

int16_t prepareFileSend()
{
    GObject         *ipAddress;
    GObject         *fileChooser;
    GtkDialog       *dialogMessage;
    GtkMessageType  messageType;
    GtkDialogFlags  flags;
    const gchar           *filePath;
    const gchar     *receiverIPAddress;
    progressbar = gtk_builder_get_object(builder, "progressbar");
    gdouble         progressbarFill;
    gdouble a = 6;
    gdouble b = 10;


    const gchar            *message;
    const gchar            *messageDescription;

    messageType         = GTK_MESSAGE_INFO;                                     // INFO or ERROR
    progressbarFill     -= a/b;


    flags       = GTK_DIALOG_DESTROY_WITH_PARENT;

    fileChooser         = gtk_builder_get_object(builder, "filechooserbutton");
    filePath            = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(fileChooser));

    ipAddress           = gtk_builder_get_object(builder, "entryIPAddress");
    receiverIPAddress   = gtk_entry_get_text (GTK_ENTRY(ipAddress));

    message             = receiverIPAddress;
    messageDescription  = filePath;



    gtk_widget_show(GTK_WIDGET(progressbar));
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progressbar), (gdouble)(a/b));
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

    gtk_widget_hide(GTK_WIDGET(progressbar));

    statusbar = gtk_builder_get_object(builder, "statusbar");
    /* get id for the message at the top of the
     * info stack? */
    guint id = gtk_statusbar_get_context_id(GTK_STATUSBAR(statusbar), "info");
    /* show the top message from the info stack
     * ? */
gtk_statusbar_pop (GTK_STATUSBAR(statusbar), id);








}
