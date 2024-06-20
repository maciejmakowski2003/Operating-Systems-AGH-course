#ifndef SPEC_H
#define SPEC_H

#define BUFFER_SIZE 1024
#define MAX_CLIENTS 8

typedef struct {
    int socket;
    char id[32];
    int active;
} client_t;

#endif //SPEC_H