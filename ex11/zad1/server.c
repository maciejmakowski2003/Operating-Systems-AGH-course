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

// Function to send the list of active clients to the requesting client
void send_client_list(int client_socket) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].active) {
            char list_message[BUFFER_SIZE];
            snprintf(list_message, sizeof(list_message), "client: %s\n", clients[i].id);
            send(client_socket, list_message, strlen(list_message), 0);
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

// Function to handle private messages between clients
void handle_private_message(const char* sender_id, const char* buffer, int sender_socket) {
    char client_id[32];
    sscanf(buffer + 5, "%s", client_id);
    char* msg = strchr(buffer + 5, ' ') + 1;

    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].active && strcmp(clients[i].id, client_id) == 0) {
            char private_message[BUFFER_SIZE];
            snprintf(private_message, sizeof(private_message), "[%s to %s]: %s", sender_id, client_id, msg);
            send(clients[i].socket, private_message, strlen(private_message), 0);
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

// Function to broadcast a message to all clients except the sender
void broadcast_message(const char* sender_id, const char* message, int sender_socket) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].active && clients[i].socket != sender_socket) {
            char buffer[BUFFER_SIZE];
            snprintf(buffer, sizeof(buffer), "[%s]: %s", sender_id, message);
            send(clients[i].socket, buffer, strlen(buffer), 0);
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

// Function to add a client to the list
void add_client(int client_socket, const char* client_id) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (!clients[i].active) {
            clients[i].socket = client_socket;
            strcpy(clients[i].id, client_id);
            clients[i].active = 1;
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

// Function to remove a client from the list and clean up
void remove_client(int client_socket) {
    close(client_socket);
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].socket == client_socket) {
            clients[i].active = 0;
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);

}

// Function to handle communication with a single client
void* client_handler(void* arg) {
    int client_socket = *((int*)arg);
    char buffer[BUFFER_SIZE];
    char client_id[32];
    int size;

    // Receive client ID
    if ((size = recv(client_socket, client_id, sizeof(client_id), 0)) <= 0) {
        perror("recv");
        close(client_socket);
        return NULL;
    }
    client_id[size] = '\0';

    // Add client to clients list
    add_client(client_socket, client_id);

    // Communication loop
    while ((size = recv(client_socket, buffer, sizeof(buffer), 0)) > 0) {
        buffer[size] = '\0';

        if (strncmp(buffer, "LIST", 4) == 0) {
            send_client_list(client_socket);
        } else if (strncmp(buffer, "2ALL", 4) == 0) {
            broadcast_message(client_id, buffer + 5, client_socket);
        } else if (strncmp(buffer, "2ONE", 4) == 0) {
            handle_private_message(client_id, buffer, client_socket);
        } else if (strncmp(buffer, "STOP", 4) == 0) {
            remove_client(client_socket);
            break;
        }
    }

    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <server port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    int port = atoi(argv[1]);

    // Create server socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    // Bind server socket
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_socket, MAX_CLIENTS) == -1) {
        perror("listen");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // Main server loop to accept and handle clients
    while (1) {
        int client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_len);
        if (client_socket == -1) {
            perror("accept");
            continue;
        }

        // Create a new thread for each client
        pthread_t thread;
        pthread_create(&thread, NULL, client_handler, (void*)&client_socket);
        pthread_detach(thread);
    }

    // Close server socket
    close(server_socket);
    return 0;
}