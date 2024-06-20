#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <mqueue.h>
#include "chat_specs.h"

int main(){
    struct mq_attr attributes;
    attributes.mq_flags = 0;
    attributes.mq_maxmsg = 10; 
    attributes.mq_msgsize = sizeof(msg_t);

    char mq_client_name[30]; 
    pid_t pid = getpid(); 
    snprintf(mq_client_name,sizeof(mq_client_name),"%s_%d",CLIENT_QUEUE,pid);

    mqd_t mq_client = mq_open(mq_client_name, O_RDWR | O_CREAT, 0600, &attributes);
    if(mq_client < 0){
        perror("open mq_client");
        exit(EXIT_FAILURE);
    }

    mqd_t mq_server = mq_open(SERVER_QUEUE, O_RDWR, 0600, &attributes);
    if(mq_server < 0){
        perror("open mq_server");
        exit(EXIT_FAILURE);
    }

    msg_t msg_init; 
    msg_init.type = INIT;
    strcpy(msg_init.msg,mq_client_name);

    mq_send(mq_server, (char*)&msg_init, sizeof(msg_init), 1);

    int id_pipe[2];
    if(pipe(id_pipe) < 0){
        perror("pipe");
    }

    pid_t receiver = fork();
    if(receiver < 0){
        perror("fork");
        exit(EXIT_FAILURE);
    }

    else if(receiver == 0){
        msg_t msg_received;
        close(id_pipe[0]);

        while(1){
            if(mq_receive(mq_client, (char*)&msg_received, sizeof(msg_received), NULL) < 0){
                perror("receive mq_server");
            }

            switch(msg_received.type){
                case ID:
                    int id = msg_received.id;
                    printf("from server got id: %d\n", id);
                    write(id_pipe[1], &id, sizeof(id));
                    break;
                case MSG:
                    printf("sender id: %d, message: %s\n", msg_received.id, msg_received.msg);
                    break;
                default:
                    printf("unknow message type\n");
                    break;
            }
        }
    }

    else{
        close(id_pipe[1]);
        int id;
        read(id_pipe[0], &id, sizeof(id));
        char* msg = NULL;

        while(1){
            if(scanf("%ms", &msg) == 1){                
                msg_t msg_send; 
                msg_send.id = id;
                msg_send.type = MSG; 

                strncpy(msg_send.msg, msg, sizeof(msg_send.msg) - 1);
                msg_send.msg[sizeof(msg_send.msg) - 1] = '\0';

                mq_send(mq_server, (char*)&msg_send, sizeof(msg_send), 1);

                free(msg);
                msg = NULL;
            }
        }
    }

    return EXIT_SUCCESS;
}