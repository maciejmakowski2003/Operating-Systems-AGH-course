#define _XOPEN_SOURCE 700
#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <signal.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <limits.h>
#include "spec.h"

int udp_socket;
struct sockaddr_in server_address;

// Cleanup function to close the socket and exit the program
void cleanup() {
    if (close(udp_socket) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}

// Signal handler to handle Ctrl+C (SIGINT) and disconnect gracefully
void handle_sigint(int sig) {
    printf("Disconnected\n");
    sendto(udp_socket, "STOP", 4, 0, (struct sockaddr*)&server_address, sizeof(server_address));
    cleanup();
}

// Thread function to receive messages from the server
void* receive_messages(void* arg) {
    char buffer[BUFFER_SIZE];
    while (1) {
        int length = recvfrom(udp_socket, buffer, BUFFER_SIZE, 0, NULL, NULL);
        if (length > 0) {
            buffer[length] = '\0';
            printf("%s\n", buffer);
            fflush(stdout);
        }
    }
    return NULL;
}

// Setup signal handler for SIGINT
void setup_signal_handler() {
    signal(SIGINT, handle_sigint);
}

// Initialize UDP socket
void initialize_udp_socket() {
    if ((udp_socket = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
}

// Setup server address structure
void setup_server_address(char* ip, int port) {
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    if (inet_pton(AF_INET, ip, &server_address.sin_addr) <= 0) {
        perror("inet_pton");
        close(udp_socket);
        exit(EXIT_FAILURE);
    }
}

// Send initial message to the server with client ID and checksum
void send_initial_message(char* client_id) {
    char init_message[BUFFER_SIZE - MAX_CHECKSUM_LENGTH - 2]; // Adjust size to account for checksum and space
    snprintf(init_message, sizeof(init_message), "%s ", client_id);
    unsigned int checksum = calculate_checksum(init_message);
    char message_with_checksum[BUFFER_SIZE];
    snprintf(message_with_checksum, sizeof(message_with_checksum), "%u %s", checksum, init_message);
    sendto(udp_socket, message_with_checksum, strlen(message_with_checksum), 0, (struct sockaddr*)&server_address, sizeof(server_address));
}

// Start thread to receive messages from the server
void start_receiving_thread(pthread_t* receive_thread) {
    if (pthread_create(receive_thread, NULL, receive_messages, NULL) != 0) {
        perror("pthread_create");
        close(udp_socket);
        exit(EXIT_FAILURE);
    }
}

// Process user input and send messages to the server
void process_user_input(char* client_id) {
    char buffer[BUFFER_SIZE - MAX_CHECKSUM_LENGTH - 2];
    char message[BUFFER_SIZE - MAX_CHECKSUM_LENGTH - 2];

    while (1) {
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            buffer[strcspn(buffer, "\n")] = '\0';

            size_t max_message_length = sizeof(message) - strlen(client_id) - 2;
            if (strlen(buffer) > max_message_length) {
                buffer[max_message_length] = '\0';
            }

            snprintf(message, sizeof(message), "%s %s", client_id, buffer);
            unsigned int checksum = calculate_checksum(message);
            char message_with_checksum[BUFFER_SIZE];
            snprintf(message_with_checksum, sizeof(message_with_checksum), "%u %s", checksum, message);
            sendto(udp_socket, message_with_checksum, strlen(message_with_checksum), 0, (struct sockaddr*)&server_address, sizeof(server_address));
        }
    }
}

int main(int argc, char* argv[]) {
    // Check for proper usage
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <client id> <server ip> <server port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char* client_id = argv[1];
    char* server_ip = argv[2];
    int server_port = atoi(argv[3]);

    // Initialize socket, server address, and signal handler
    initialize_udp_socket();
    setup_server_address(server_ip, server_port);
    send_initial_message(client_id);
    setup_signal_handler();

    // Start receiving thread and process user input
    pthread_t receive_thread;
    start_receiving_thread(&receive_thread);
    process_user_input(client_id);

    pthread_join(receive_thread, NULL);

    cleanup();

    return 0;
}