/**
 * server.c -- a stream socket server demo
 */

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
#include <sys/stat.h>
#include <fcntl.h>

#define PORT            "3490"      // the port users wil be connecting to 
#define BACKLOG         10          // how many pending connection queue will hold
#define FILE_TO_SEND    "hello.c"

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

int main(void)
{
    int                     sockfd, new_fd;     // listen on sockd_fd, new connection on new_fd
    struct addrinfo         hints, *servinfo, *p;
    struct sockaddr_storage their_addr;         // connector's address information 
    socklen_t               sin_size;
    struct sigaction        sa;
    int                     yes = 1;
    char                    s[INET6_ADDRSTRLEN];
    int                     rv;
    
    struct stat             file_stat;
    int                     offset;
    int                     remain_data;
    int                     sent_bytes;
    char                    file_size[256];
    int                     fd;

    memset(&hints, 0, sizeof hints);
    hints.ai_family     = AF_UNSPEC;
    hints.ai_socktype   = SOCK_STREAM;
    hints.ai_flags      = AI_PASSIVE;        // use my IP

    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
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

    fd = open(FILE_TO_SEND, O_RDONLY);
    if (fd == -1)
    {
        fprintf(stderr, "Error opening file --> %s", strerror(errno));
        exit(1);
    }

    if (fstat(fd, &file_stat) < 0)
    {
        fprintf(stderr, "Error fstat --> %s", strerror(errno));
        exit(1);
    }

    fprintf(stdout, "File size: \n%d bytes\n", file_stat.st_size);
    sprintf(file_size, "%d", file_stat.st_size);

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
            if (send(new_fd, file_size, sizeof(file_size), 0) == -1)
            {
                fprintf(stderr, "Error on sending greetings --> %s", strerror(errno));
            }

            offset = 0; 
            remain_data = file_stat.st_size;
            /* Sending file data */
            sent_bytes = sendfile(new_fd, fd, &offset, BUFSIZ);
            printf("sent bytes %d\n", sent_bytes);

            while (((sent_bytes = sendfile(new_fd, fd, &offset, BUFSIZ)) > 0) && (remain_data > 0))
            {
                printf("was in loop");     
                fprintf(stdout, "1. Server sent %d bytes from file's data, offset is now : %d and remaining data = %d\n", sent_bytes, offset, remain_data);
                remain_data -= sent_bytes;
                fprintf(stdout, "2. Server sent %d bytes from file's data, offset is now : %d and remaining data = %d\n", sent_bytes, offset, remain_data);
            }

            close(new_fd);
            exit(0);
        }
        close(new_fd);      // parent doesn't need this
    }


    return 0;
}


 
