#include "tcp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main(void)
{
    tcpSocket_init();

    tcpSocket_t* socketClient = tcpSocket_create();
    if (!socketClient)
    {
        fprintf(stderr, "Failed to create client socket\n");
        return EXIT_FAILURE;
    }

    if (tcpSocket_connect(socketClient, "127.0.0.1", PORT) != 0)
    {
        fprintf(stderr, "Failed to connect to server\n");
        tcpSocket_close(socketClient);
        return EXIT_FAILURE;
    }

    printf("Connected to server\n");

    while (true)
    {
        char message[BUFFER_SIZE];
        printf("Enter message (type 'exit' to quit): ");
        if (fgets(message, sizeof(message), stdin) == NULL)
        {
            perror("fgets");
            break;
        }

        // Remove newline character if present
        message[strcspn(message, "\n")] = '\0';

        // Send message to server
        if (tcpSocket_send(socketClient, message, strlen(message)) < 0)
        {
            fprintf(stderr, "Failed to send data to server\n");
            break;
        }

        if (strcmp(message, "exit") == 0)
        {
            printf("Exiting...\n");
            break;
        }

        // Receive response from the server
        char buffer[BUFFER_SIZE];
        size_t bytes_received = tcpSocket_receive(socketClient, buffer, sizeof(buffer) - 1);
        if (bytes_received > 0)
        {
            buffer[bytes_received] = '\0'; 
            printf("Received from server: %s\n", buffer);
        }
        else
        {
            printf("Server disconnected or error\n");
            break;
        }
    }

    // Close the socket
    tcpSocket_close(socketClient);

    return EXIT_SUCCESS;
}