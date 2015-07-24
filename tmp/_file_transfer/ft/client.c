/**
 * client.c -- a stream socket client demo
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT        "3490"      // the port client will be connecting to
#define MAXDATASIZE 100         // max number of bytes we can get at once
#define FILENAME    "foo.c"

// get socck addr, IPv4 or IPv6:
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
    int             sockfd, numbytes;
    char            buf[MAXDATASIZE];
    struct addrinfo hints, *servinfo, *p;
    int             rv;
    char            s[INET6_ADDRSTRLEN];

    ssize_t         len;
    char            buffer[BUFSIZ];
    int             file_size;
    FILE            *received_file;
    int             remain_data = 0;

    memset(&hints, 0, sizeof hints);
    hints.ai_family     = AF_UNSPEC;
    hints.ai_socktype   = SOCK_STREAM;
    hints.ai_flags      = AI_PASSIVE;        // use my IP
    
    if ((rv = getaddrinfo(argv[1], PORT, &hints, &servinfo)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through al the results and connect to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) 
    {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
        {
            perror("client: socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1)
        {
            close(sockfd);
            perror("client: connect");
            continue;
        }

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

    if ((numbytes = recv(sockfd, buffer, BUFSIZ, 0)) == -1)
    {
        perror("recv");
        exit(1);
    }
    file_size = atoi(buffer);
    fprintf(stdout, "\nFile size : %d\n", file_size);

    
    received_file = fopen(FILENAME, "w");
    if (received_file == NULL)
    {
            fprintf(stderr, "Failed to open file foo --> %s\n", strerror(errno));

            exit(EXIT_FAILURE);
    }

    remain_data = file_size;
    len = recv(sockfd, buffer, BUFSIZ, 0);
    printf("size recv %d\n", len );

    while (((len = recv(sockfd, buffer, BUFSIZ, 0)) > 0) && (remain_data > 0))
    {
            fwrite(buffer, sizeof(char), len, received_file);
            remain_data -= len;
            fprintf(stdout, "Receive %d bytes and we hope :- %d bytes\n", len, remain_data);
    }
    fclose(received_file);

    close(sockfd);


    return 0;
}

    
