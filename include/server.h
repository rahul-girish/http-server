#ifndef SERVER_H
#define SERVER_H

#include <sys/socket.h>

struct server
{
    int port;                        // port number
    int backlog;                     // max queue size
    struct sockaddr_storage address; // address storage
    socklen_t address_len;           // len of address
    int sockfd;                      // file descriptor

    void (*start)(struct server *); // start function
};

struct server server_init(int port, int backlog,
                          void (*start)(struct server *));
#endif