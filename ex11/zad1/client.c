#define _XOPEN_SOURCE 700
#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "spec.h"

// Global variable to hold the server socket descriptor
int server_socket;

// Cleanup function to close the server socket and exit
void cleanup() {
    if (close(server_socket) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}

// Signal handler for SIGINT (Ctrl+C)
void handle_sigint(int sig) {
    char *msg = "STOP";
    printf("Disconnected\n");
    send(server_socket, msg, strlen(msg), 0);
    cleanup();
}

int main(int argc, char *argv[]) {
    // Check for the correct number of command-line arguments
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <client id> <server ipv4> <server port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Parse command-line arguments
    char *client_id = argv[1];
    char *server_ipv4 = argv[2];
    int server_port = atoi(argv[3]);

    // Create a socket for the client
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Define the server address structure
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);

    // Convert the server IPv4 address from text to binary form
    if (inet_pton(AF_INET, server_ipv4, &server_addr.sin_addr) <= 0) {
        perror("inet_pton");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // Connect to the server
    if (connect(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // Set up the SIGINT handler
    signal(SIGINT, handle_sigint);

    // Buffer for sending and receiving messages
    char buffer[BUFFER_SIZE];
    
    // File descriptor set for select
    fd_set readfds;

    // Send the client ID to the server
    if (send(server_socket, client_id, strlen(client_id), 0) == -1) {
        perror("send");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    while (1) {
        // Initialize the set of file descriptors
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);
        FD_SET(server_socket, &readfds);

        // Wait for an activity on one of the file descriptors
        if (select(server_socket + 1, &readfds, NULL, NULL, NULL) == -1) {
            perror("select");
            cleanup();
        }

        // Check if there is input from stdin
        if (FD_ISSET(STDIN_FILENO, &readfds)) {
            if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
                if (send(server_socket, buffer, strlen(buffer), 0) == -1) {
                    perror("send");
                    cleanup();
                }
            }
        }

        // Check if there is data from the server
        if (FD_ISSET(server_socket, &readfds)) {
            int size = recv(server_socket, buffer, sizeof(buffer) - 1, 0);
            if (size > 0) {
                buffer[size] = '\0';
                printf("%s", buffer);
            } else if (size == 0) {
                // Server closed the connection
                printf("Server disconnected.\n");
                cleanup();
            } else {
                perror("recv");
                cleanup();
            }
        }
    }

    cleanup();
    return 0;
}