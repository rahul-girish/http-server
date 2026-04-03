#include "server.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void start(struct server *server)
{

    char buffer[4096];
    char *hello =
        "HTTP/1.1 200 OK\r\nDate: Fri, 6 Mar 2026 12:21:36 GMT\r\n Server: "
        "Apache/2.2.14 (Win32)\r\nLast-Modified: Fri, 6 Mar 2026 12:20:03 "
        "GMT\r\nContent-Length: 47\r\nContent-Type: text/html\nConnection: "
        "Closed\r\n\r\n<html><body><h1>Hello, world</h1></body></html>";
    int new_sockfd;

    /* `buffer` stores the data we read from the client file descriptor .
    `hello` is a simple hello world message.
    `new_sockfd` is the client file descriptor */

    /* wait for connections */
    while (1)
    {
        printf("server: waiting for connections...\n");

        /* accept a connection */
        new_sockfd =
            accept(server->sockfd, (struct sockaddr *)&(server->address),
                   &(server->address_len));

        /* clear the buffer and read into it*/
        memset(buffer, 0, sizeof(buffer));
        read(new_sockfd, buffer, sizeof(buffer));
        printf("%s\n", buffer);

        /* write our hello world message to the client and close the
         * connection*/
        write(new_sockfd, hello, strlen(hello));
        close(new_sockfd);
    }
}

int main(void)
{
    struct server server = server_init(8080, 10, &start);
    server.start(&server);
    return 0;
}