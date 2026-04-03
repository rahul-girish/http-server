#include "server.h"
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct server server_init(int port, int backlog, void (*start)(struct server *))
{
    /* create a new struct and initialize the fields */
    struct server server;

    server.port = port;
    server.backlog = backlog;
    server.start = start;

    /* get address info, then attempt to create and bind a socket */

    struct addrinfo hints, *servinfo, *p;
    int status;
    int yes = 1;

    /* `hints` is a struct that tells getaddrinfo() what kind of addresses we
     * want.
     * `servinfo` will store a linked list of options.
     * `p` is an iterator.
     * `status` stores the return value of getaddrinfo().
     * `yes` is used to enable SO_REUSEADDR via setsockopt() */

    memset(&hints, 0, sizeof(hints)); // zero everything out
    hints.ai_family = AF_UNSPEC;      // both IPv4 and IPv6
    hints.ai_socktype = SOCK_STREAM;  // specifies TCP socket
    hints.ai_flags = AI_PASSIVE;      // `AI_PASSIVE` with `NULL` means bind to
                                      // all local interfaces

    char port_str[8]; // 8 bytes for a port string
    snprintf(port_str, sizeof(port_str), "%d", server.port); // convert to str

    /* find local interfaces suitable for binding */
    if ((status = getaddrinfo(NULL, port_str, &hints, &servinfo)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
    }
    /* `NULL` with `AI_PASSIVE` indicates that we are getting the addresses
     * suitable for binding on this machine. `servinfo` now points to a linked
     * list of `struct addrinfo`. also, if the status is not 0, we print the
     * error message. */

    /* loop through the linked list and try to create and bind a socket */
    for (p = servinfo; p != NULL; p = p->ai_next)
    {
        if ((server.sockfd =
                 socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
        {
            perror("server: socket");
            continue;
        }

        /* enable reusing the port immediately */
        if (setsockopt(server.sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                       sizeof(int)) == -1)
        {
            perror("server: setsockopt");
            exit(1);
        }

        /* bind socket to the address*/
        if (bind(server.sockfd, p->ai_addr, p->ai_addrlen) == -1)
        {
            close(server.sockfd);
            perror("server: bind");
            continue;
        }

        /* if nothing failed, store the address used for binding */
        memcpy(&server.address, p->ai_addr, p->ai_addrlen);
        server.address_len = p->ai_addrlen;
        break;
    }

    /* free the linked list*/
    freeaddrinfo(servinfo);

    /* if none of the attempts to bind succeeded */
    if (p == NULL)
    {
        fprintf(stderr, "server: failed to bind\n");
        return (struct server){0};
    }

    /* start listening */
    if (listen(server.sockfd, server.backlog) == -1)
    {
        perror("server: listen");
        return (struct server){0};
    }

    return server;
}