#define _XOPEN_SOURCE 700
#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>
#include <time.h>
#include "spec.h"

// Global variables
client_t clients[MAX_CLIENTS];
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;
int server_socket;

// Function to broadcast a message to all clients except the sender
void broadcast_message(const char* sender_id, const char* message, struct sockaddr_in* exclude_address) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].active && (exclude_address == NULL || memcmp(&clients[i].addr, exclude_address, sizeof(struct sockaddr_in)) != 0)) {
            char buffer[BUFFER_SIZE];
            snprintf(buffer, sizeof(buffer), "[%s]: %s", sender_id, message);
            ssize_t sent_bytes = sendto(server_socket, buffer, strlen(buffer), 0, (struct sockaddr*)&clients[i].addr, sizeof(clients[i].addr));
            if (sent_bytes == -1) {
                perror("sendto");
            }
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

// Function to handle private messages between clients
void handle_private_message(const char* sender_id, const char* buffer, struct sockaddr_in* sender_address) {
    char target_id[32];
    sscanf(buffer + 5, "%s", target_id);
    char* msg = strchr(buffer + 5, ' ') + 1;

    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].active && strcmp(clients[i].id, target_id) == 0) {
            char private_message[BUFFER_SIZE];
            snprintf(private_message, sizeof(private_message), "[%s to %s]: %s", sender_id, target_id, msg);
            sendto(server_socket, private_message, strlen(private_message), 0, (struct sockaddr*)&clients[i].addr, sizeof(clients[i].addr));
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

// Function to send the list of active clients to the requesting client
void send_client_list(struct sockaddr_in* client_address) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].active) {
            char list_message[BUFFER_SIZE];
            snprintf(list_message, sizeof(list_message), "Client: %s\n", clients[i].id);
            sendto(server_socket, list_message, strlen(list_message), 0, (struct sockaddr*)client_address, sizeof(*client_address));
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

// Function to handle incoming client messages
void handle_client_message(char* message, int message_length, struct sockaddr_in* client_address) {
    char checksum_str[16];
    sscanf(message, "%s ", checksum_str);
    unsigned int received_checksum = atoi(checksum_str);

    char* actual_message = strchr(message, ' ') + 1;
    unsigned int calculated_checksum = calculate_checksum(actual_message);

    if (received_checksum != calculated_checksum) {
        fprintf(stderr, "Checksum mismatch: received %u, calculated %u\n", received_checksum, calculated_checksum);
        return; // Discard the message
    }

    // Process the message as before
    char client_id[32];
    char command[BUFFER_SIZE];
    sscanf(actual_message, "%s %[^\n]", client_id, command);

    pthread_mutex_lock(&clients_mutex);
    int i;
    int found = 0;
    for (i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].active && memcmp(&clients[i].addr, client_address, sizeof(struct sockaddr_in)) == 0) {
            found = 1;
            break;
        }
    }
    if (!found) {
        for (i = 0; i < MAX_CLIENTS; i++) {
            if (!clients[i].active) {
                clients[i].addr = *client_address;
                strcpy(clients[i].id, client_id);
                clients[i].active = 1;
                break;
            }
        }
    }
    pthread_mutex_unlock(&clients_mutex);

    if (strncmp(command, "LIST", 4) == 0) {
        send_client_list(client_address);
    } else if (strncmp(command, "2ALL", 4) == 0) {
        broadcast_message(client_id, command + 5, client_address);
    } else if (strncmp(command, "2ONE", 4) == 0) {
        handle_private_message(client_id, command, client_address);
    } else if (strncmp(command, "STOP", 4) == 0) {
        pthread_mutex_lock(&clients_mutex);
        for (i = 0; i < MAX_CLIENTS; i++) {
            if (clients[i].active && memcmp(&clients[i].addr, client_address, sizeof(struct sockaddr_in)) == 0) {
                clients[i].active = 0;
                break;
            }
        }
        pthread_mutex_unlock(&clients_mutex);
    }
}

// Signal handler to shutdown the server
void handle_sigint(int signal) {
    printf("\nShut down\n");
    close(server_socket);
    exit(EXIT_SUCCESS);
}

// Function to initialize the server socket
void initialize_server_socket(int port) {
    if ((server_socket = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(port);

    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        perror("bind");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", port);
}

// Function to start the server and handle incoming messages
void start_server() {
    struct sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);
    char buffer[BUFFER_SIZE];

    while (1) {
        int message_length = recvfrom(server_socket, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&client_address, &client_address_len);
        if (message_length > 0) {
            buffer[message_length] = '\0';
            handle_client_message(buffer, message_length, &client_address);
        }
    }
}

// Main function to setup the server
int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <server port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int server_port = atoi(argv[1]);

    signal(SIGINT, handle_sigint);
    initialize_server_socket(server_port);
    start_server();

    close(server_socket);
    return 0;
}
