 #ifndef TCP_H
#define TCP_H

#include <stddef.h>
#include <stdint.h>
#include "vector.h"

#define TCP_MAX_PAYLOAD_SIZE 1024

typedef struct tcpSocket
{
    int socket_fd;  // File descriptor for the socket
} tcp_socket_t;

// Initializes the TCP library
void tcp_init(void);

// Creates a new TCP socket and returns a handle
tcp_socket_t* tcp_create_socket(void);

// Binds a TCP socket to an address and port
int tcp_bind(tcp_socket_t* sock, const char* address, uint16_t port);

// Listens for incoming connections on a TCP socket
int tcp_listen(tcp_socket_t* sock, int backlog);

// Accepts an incoming connection and returns a new socket handle
tcp_socket_t* tcp_accept(tcp_socket_t* server_sock);

// Connects to a remote TCP server
int tcp_connect(tcp_socket_t* sock, const char* address, uint16_t port);

// Sends data through a TCP socket
size_t tcp_send(tcp_socket_t* sock, const void* data, size_t length);

// Receives data from a TCP socket
size_t tcp_recive(tcp_socket_t* sock, void* buffer, size_t length);

// Closes a TCP socket
void tcp_close(tcp_socket_t* sock);

// Manages a vector of TCP sockets
typedef struct
{
    struct Vector* sockets;
    struct Vector* poll_fds;  // Vector for pollfd structs
} tcp_socket_manager_t;

// Initializes a TCP socket manager
tcp_socket_manager_t* tcp_socketManager_init(void);

// Adds a TCP socket to the manager
void tcp_socketManager_add(tcp_socket_manager_t* manager, tcp_socket_t* sock);

// Removes a TCP socket from the manager
void tcp_socketManager_remove(tcp_socket_manager_t* manager, tcp_socket_t* sock);

// Frees the TCP socket manager
void tcp_socketManager_free(tcp_socket_manager_t* manager);

#endif // TCP_H
