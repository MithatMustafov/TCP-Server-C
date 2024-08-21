#include "tcp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main(void)
{
    tcpSocket_init();

    // Create a new TCP socket
    tcpSocket_t* socketServer = tcpSocket_create();
    if(!socketServer)
    {
        fprintf(stderr, "Failed to create server socket.\n");
        return EXIT_FAILURE;
    }

    // Bind the socket to an address and port
    if(tcpSocket_bind(socketServer,"127.0.0.1",PORT) != 0)
    {
        fprintf(stderr, "Faild to bind server socket\n");
        tcpSocket_close(socketServer);
        return EXIT_FAILURE;
    }

    // Listen for incoming connections
    if (tcpSocket_listen(socketServer, 5) != 0)
    {
        fprintf(stderr, "Failed to listen on server socket\n");
        tcpSocket_close(socketServer);
        return EXIT_FAILURE;
    }

    printf("Server listening on port %d...\n", PORT);

    while (true)
    {
        tcpSocket_t* socketClient = tcpSocket_accept(socketServer);
        if (!socketClient)
        {
            fprintf(stderr, "Failed to accept client connection\n");
            continue;
        }

        printf("Client connected\n");
        while(true)
        {
            char buffer[BUFFER_SIZE];
            size_t bytesReceived = tcpSocket_receive(socketClient, buffer, sizeof(buffer) - 1);

            if(bytesReceived > 0)
            {
                buffer[bytesReceived] = '\0';
                printf("Received from client: %s\n", buffer);

                if (strcmp(buffer, "exit") == 0)
                {
                    printf("Closing connection with client\n");
                    break;
                }

                const char* response = "Message received";
                tcpSocket_send(socketClient, response, strlen(response));
            }
            else
            {
                printf("Client disconnected or error\n");
                break;
            }
        }
    }

    return EXIT_SUCCESS;
}