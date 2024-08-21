#ifndef TCP_H
#define TCP_H

#include <stddef.h>
#include <stdint.h>
#include "vector.h"

#define TCP_MAX_PAYLOAD_SIZE 1024

typedef struct
{
    // File descriptor for the socket
    int socket_FileDescriptor;  
} tcpSocket_t;

// Initializes the TCP library
void tcpSocket_init(void);

// Creates a new TCP socket and returns a handle
tcpSocket_t* tcpSocket_create(void);

// Binds a TCP socket to an address and port
int tcpSocket_bind(tcpSocket_t* socket, const char* address, uint16_t port);

// Listens for incoming connections on a TCP socket
int tcpSocket_listen(tcpSocket_t* socket, int backlog);

// Accepts an incoming connection and returns a new socket handle
tcpSocket_t* tcpSocket_accept(tcpSocket_t* socketServer);

// Connects to a remote TCP server
int tcpSocket_connect(tcpSocket_t* socket, const char* address, uint16_t port);

// Sends data through a TCP socket
size_t tcpSocket_send(tcpSocket_t* socket, const void* data, size_t length);

// Receives data from a TCP socket
size_t tcpSocket_receive(tcpSocket_t* socket, void* buffer, size_t length);

// Closes a TCP socket
void tcpSocket_close(tcpSocket_t* socket);

// Manages a vector of TCP sockets
typedef struct
{
    struct Vector* sockets;
    struct Vector* poll_FileDescriptor;  // Vector for pollfd structs
} tcpSocketManager_t;

// Initializes a TCP socket manager
tcpSocketManager_t* tcpSocketManager_init(void);

// Adds a TCP socket to the manager
void tcpSocketManager_add(tcpSocketManager_t* manager, tcpSocket_t* socket);

// Removes a TCP socket from the manager
void tcpSocketManager_remove(tcpSocketManager_t* manager, tcpSocket_t* socket);

// Frees the TCP socket manager
void tcpSocketManager_free(tcpSocketManager_t* manager);

#endif // TCP_H
