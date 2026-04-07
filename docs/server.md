# `struct server`

This struct contains most of the code for the socket programming. It's a lot of information to take in, but once you understand it and it all makes sense, you'll be quite happy with yourself.

Note that this particular definition is IP version-agnostic.

The definition is as follows:
```c
struct server
{
	int port;
	int backlog;
	struct sockaddr_storage address;
	socklen_t address_len;
	int sockfd;

	void (*start)(struct server*);
};
```

`port` is the port number.\
`backlog` is the maximum number of pending connections that the queue can hold.\
`address` is storage for either an IPv4 or IPv6 address.\
`address_len` is the length of the address in bytes.\
`sockfd` is the server's socket file descriptor.\
`void start(struct Server*)` is a function pointer to a user defined function that runs the server.

There is also an init function for the server struct:
```c
struct Server server_init(int port, int backlog, void (*start)(struct Server*));
```

An example of how to use this struct is given in `tests/server_test.c`. The comments in the source files should also prove useful.