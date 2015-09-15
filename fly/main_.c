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

int main(int argc, char *argv[])
{
    int                     sockfd, new_fd;     // listen on sockd_fd, new connection on new_fd
    struct addrinfo         hints, *servinfo, *p;
    struct sockaddr_storage their_addr;         // connector's address information
    socklen_t               sin_size;
    struct sigaction        sa;
    int                     yes = 1;
    char                    s[INET6_ADDRSTRLEN];
    int                     rv;
    int                     numbytes;
    char                    buf[MAXDATASIZE];
    FILE                    *file;
    char                    *buffer;                                            // For transferring characters
    ssize_t                 byteSend;                                           // Byte send to receiver
    ssize_t                 byteReceive;
    char                    filename[SIZE_FILENAME];


    buffer = (char *)malloc(MAXDATASIZE * sizeof(char));



    if (argc < 2 || argc > 3)
    {
        printf("Invalid! number of arguments\n");
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family     = AF_UNSPEC;
    hints.ai_socktype   = SOCK_STREAM;
    hints.ai_flags      = AI_PASSIVE;        // use my IP


    switch (argc)
    {
        case 2:
        {
            if ((rv = getaddrinfo(argv[1], PORT, &hints, &servinfo)) != 0)
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

            printf("file received\n");
            close(sockfd);

            break;
        }

        case 3:
        {
            if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0)
            {
                fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
                return 1;
            }

            file = fopen(argv[2], "r");

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

            while (1)
            {
                sin_size = sizeof their_addr;
                new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
                if (new_fd == -1)
                {
                    perror("accept");
                    continue;
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

                    buffer = get_filename_from_path(argv[2]);
                    byteSend = send(new_fd, buffer, MAXDATASIZE, 0);


                    printf("file successfully send");
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
            }


            break;
        }
    }



    return 0;
}


