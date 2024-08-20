#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "vector.h"
#include "tcp.h"

// Initialize WinSocket
void tcpSocket_init(void)
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        fprintf(stderr, "Failed to initialize Winsock\n");
        exit(EXIT_FAILURE);
    }
}

// Create a new TCP Socket
tcpSocket_t* tcpSocket_create(void)
{
    tcpSocket_t* newTcpSocket = (tcpSocket_t*)malloc(sizeof(tcpSocket_t));

    if (!newTcpSocket)
    {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }

    //Define a Socket
    newTcpSocket->socket_FileDescriptor = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

    if (newTcpSocket->socket_FileDescriptor == INVALID_SOCKET)
    {
        fprintf(stderr, "Socket creation FAILED: %d\n", WSAGetLastError());
        free(newTcpSocket);
        return NULL;
    }

    return newTcpSocket;
}

// Bind a TCP socket to an address and port
int tcpSocket_bind(tcpSocket_t* socket, const char* address, uint16_t port)
{
    // Specify an endpoint address in the Internet domain
    struct sockaddr_in socketEndpointAddress;
    socketEndpointAddress.sin_family = AF_INET;
    socketEndpointAddress.sin_port = htons(port);
    inet_pton(AF_INET, address, &socketEndpointAddress.sin_addr);

    if (bind(
            socket->socket_FileDescriptor,
            (struct sockaddr*)&socketEndpointAddress,
            sizeof(socketEndpointAddress)
        ) == SOCKET_ERROR)
    {
        fprintf(stderr, "Bind failed: %d\n", WSAGetLastError());
        return -1;
    }

    return 0; 
}

// Listen for incoming connections
int tcpSocket_listen(tcpSocket_t* socket, int backlog)
{
    if(listen(socket->socket_FileDescriptor, backlog) == SOCKET_ERROR)
    {
        fprintf(stderr, "Listen failed: %d\n", WSAGetLastError());
        return -1;
    }

    return 0;
}

//TCP Server Socket Accept an incoming connection
tcpSocket_t* tcpSocket_accept(tcpSocket_t* socketServer)
{
    tcpSocket_t* socketClient = (tcpSocket_t*)malloc(sizeof(tcpSocket_t));
    if (!socketClient)
    {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }

    struct sockaddr_in socketClientEndpointAddress;
    int socketCEAsize = sizeof(socketClientEndpointAddress);

    socketClient->socket_FileDescriptor = accept(
        socketServer->socket_FileDescriptor,
        (struct sockaddr*)&socketClientEndpointAddress,
        &socketCEAsize
    );

    if (socketClient->socket_FileDescriptor == INVALID_SOCKET)
    {
        fprintf(stderr, "Accept failed: %d\n", WSAGetLastError());
        free(socketClient);
        return NULL;
    }

    return socketClient;
}

// Connect to a remote TCP server
int tcpSocket_connect(tcpSocket_t* socket, const char* address, uint16_t port)
{
    struct sockaddr_in socketEndpointAddress;
    socketEndpointAddress.sin_family = AF_INET;
    socketEndpointAddress.sin_port = htons(port);
    inet_pton(AF_INET, address, &socketEndpointAddress.sin_addr);

    if (connect(
            socket->socket_FileDescriptor,
            (struct sockaddr*)&socketEndpointAddress,
            sizeof(socketEndpointAddress)
        ) == SOCKET_ERROR)
    {
        fprintf(stderr, "Connect failed: %d\n", WSAGetLastError());
        return -1;
    }

    return 0;
}

// Send data through a TCP socket
size_t tcpSocket_Send(tcpSocket_t* socket, const void* data, size_t length)
{
    int result = send(socket->socket_FileDescriptor, (const char*)data, (int)length, 0);
    
    if (result == SOCKET_ERROR)
    {
        fprintf(stderr, "Send failed: %d\n", WSAGetLastError());
        return -1;
    }

    return result;
}

// Receive data from a TCP socket
size_t tcpSocket_recive(tcpSocket_t* socket, void* buffer, size_t length)
{
    int result = recv(socket->socket_FileDescriptor, (char*)buffer, (int)length, 0);
    if (result == SOCKET_ERROR) {
        fprintf(stderr, "Receive failed: %d\n", WSAGetLastError());
        return -1;
    }
    return result;
}

// Close a TCP socket
void tcpSocket_close(tcpSocket_t* socket)
{
    closesocket(socket->socket_FileDescriptor);
    free(socket);
}

// Initializes a TCP socket manager
tcpSocketManager_t* tcpSocketManager_init(void)
{
    tcpSocketManager_t* newTcpSocketManager = (tcpSocketManager_t*)malloc(sizeof(tcpSocketManager_t));
    if (!newTcpSocketManager)
    {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }

   // Initialize sockets vector with default element size and capacity
    newTcpSocketManager->sockets = vector_init(sizeof(tcpSocketManager_t*), 1);
    if (!newTcpSocketManager->sockets) {
        free(newTcpSocketManager);
        return NULL;
    }

    // Initialize poll_fds vector with default element size and capacity
    newTcpSocketManager->poll_FileDescriptor = vector_init(sizeof(struct pollfd), 1);
    if (!newTcpSocketManager->poll_FileDescriptor) {
        vector_free(newTcpSocketManager->sockets);
        free(newTcpSocketManager);
        return NULL;
    }

    return newTcpSocketManager;
}

// Add a TCP socket to the manager
void tcpSocketManager_add(tcpSocketManager_t* tcpSocketManager, tcpSocket_t* socket)
{
    vector_push(tcpSocketManager->sockets, socket);
    struct pollfd pfd;
    pfd.fd = socket->socket_FileDescriptor;
    pfd.events = POLLIN;
    vector_push(tcpSocketManager->poll_FileDescriptor, &pfd);
}

void tcpSocketManager_remove(tcpSocketManager_t* tcpSocketManager, tcpSocket_t* socket)
{
    //Remove the TCP socket from the sockets vector
    size_t socketsLength = tcpSocketManager->sockets->getLength;
    for (size_t index = 0; index < socketsLength; index++)
    {
        tcpSocket_t* currentSocket =
        (tcpSocket_t*)tcpSocketManager->sockets->get(tcpSocketManager->sockets, socketsLength - 1);

        if (currentSocket == socket)
        {
            if(index < socketsLength - 1)
            {
                tcpSocket_t* lastSock =
                *(tcpSocket_t**)vector_get(tcpSocketManager->sockets, socketsLength - 1);
                
                tcpSocketManager->sockets->set(tcpSocketManager->sockets, index, &lastSock);
            }
            
            tcpSocketManager->sockets->set(tcpSocketManager->sockets, socketsLength - 1, NULL);
            tcpSocketManager->sockets->length--;
            break;
        }
    }

    //Remove the pollfd from the poll_fds vector
    size_t pollFdsLength = vector_getLength(tcpSocketManager->poll_FileDescriptor);
    for (size_t index = 0; index < pollFdsLength; index++)
    {
        struct pollfd* pfd =
        (struct pollfd*)tcpSocketManager->poll_FileDescriptor->get(tcpSocketManager->poll_FileDescriptor, index);
        if (pfd->fd == socket->socket_FileDescriptor)
        {
           
            if (index < pollFdsLength - 1)
            {
                struct pollfd* lastPfd =
                (struct pollfd*)tcpSocketManager->poll_FileDescriptor->get(tcpSocketManager->poll_FileDescriptor, pollFdsLength - 1);
                tcpSocketManager->poll_FileDescriptor->set(tcpSocketManager->poll_FileDescriptor, index, lastPfd);
            }

            tcpSocketManager->poll_FileDescriptor->set(tcpSocketManager->poll_FileDescriptor, pollFdsLength - 1, NULL);
            tcpSocketManager->poll_FileDescriptor->length--; 
            break;
        }
    }
    // Close the socket
    tcpsocket_close(socket);
}

// Frees the TCP socket manager
void tcpSocketManager_free(tcpSocketManager_t* tcpSocketManager)
{
    for (size_t i = 0; i < vector_getLength(tcpSocketManager->sockets); i++)
    {
        tcpSocket_t* socket = vector_get(tcpSocketManager->sockets, i);
        tcp_close(socket);
    }
    vector_free(tcpSocketManager->sockets);
    vector_free(tcpSocketManager->poll_FileDescriptor);
    free(tcpSocketManager);
}
