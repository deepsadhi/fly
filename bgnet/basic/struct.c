struct addrinfo {
    int             ai_flags;       // AI_PASSIVE, AI_CANONNAME, etc.
    int             ai_family;      // AF_INET, AF_INET6, AF_UNSPEC
    int             ai_socktype;    // SOCK_STREAM, SOCK_DGRAM
    int             ai_protocal;    // use 0 for "any"
    size_t          ai_addrlen;     // size of ai_addr in bytes
    struct sockaddr *ai_addr;       // struct sockaddr_in or _in6
    char            *ai_canonname;  // full canonical hostname

    struct addrinfo *ai_next;       // linked list, next node
};

struct sockaddr {
    unsigned short  sa_family;      // address family, AF_xxx
    char            sa_data[14];    // 14 bytes of protocal address
};

struct sockaddr_in {
    short int           sin_family; // Address family, AF_INET
    unsigned short int  sin_port;   // Port number
    struct in_addr      sin_addr;   // Internet address
    unsigned char       siin_zero[8]; // Same size of as struct sockaddr
};

struct in_addr {
    uint32_t            s_addr;     // that's a 32-bit int (4 bytes)
};

struct sockaddr_in sa;
inet_pton(AF_INET, "192.0.2.1", &(sa.sin_addr)); // IPv4

char ip4[INET_ADDRSTRLEN];  // sapce to hold the IPv4 string
struct sockaddr_in sa;      // pretend this is loaded with something

inet_ntop(AF_INET, &(sa.sin_addr), ip4, INET_ADDRSTRLEN);
printf("The IPv4 address is: %s\n", ip4);


