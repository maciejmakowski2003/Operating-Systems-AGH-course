#ifndef SPEC_H
#define SPEC_H

#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024
#define MAX_CHECKSUM_LENGTH 10
#define MAX_CLIENTS 8

typedef struct {
    struct sockaddr_in addr;
    char id[32];
    int active;
} client_t;

unsigned int calculate_checksum(const char *message) {
    unsigned int checksum = 0;
    for (size_t i = 0; i < strlen(message); ++i) {
        checksum += message[i];
    }
    return checksum;
}

#endif //SPEC_H
