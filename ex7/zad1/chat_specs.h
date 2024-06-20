#ifndef CHAT_SPECS
#define CHAT_SPECS

#define BUFFER_SIZE 2048
#define SERVER_QUEUE "/server_queue"
#define CLIENT_QUEUE "/client_queue"
#define MAX_CLIENTS 10

typedef enum{
    INIT,
    MSG,
    ID
} msg_type_t;

typedef struct{
    msg_type_t type;
    int id; 
    char msg[BUFFER_SIZE];
} msg_t;

#endif