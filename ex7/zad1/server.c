#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <mqueue.h>
#include <sys/stat.h>
#include "chat_specs.h"

int main(){
    struct mq_attr attributes;
    attributes.mq_flags = 0;
    attributes.mq_maxmsg = 10; 
    attributes.mq_msgsize = sizeof(msg_t);

    mqd_t mq_server = mq_open(SERVER_QUEUE, O_RDWR | O_CREAT,  0600, &attributes);
    if(mq_server < 0){
        perror("open mq_server");
        exit(EXIT_FAILURE);
    }

    int clients_count = 0;
    mqd_t mq_clients[MAX_CLIENTS];
    msg_t msg_received; 

    while(1){
        if(mq_receive(mq_server, (char*)&msg_received, sizeof(msg_received), NULL) < 0){
            perror("receive mq_server");
        }

        switch(msg_received.type){
            case INIT:
                mq_clients[clients_count] = mq_open(msg_received.msg, O_RDWR, 0600, NULL);
                if(mq_clients[clients_count] < 0){
                    perror("open mq_clients[clients_count]");
                }

                msg_t msg_send; 
                msg_send.type = ID; 
                msg_send.id = clients_count;  

                mq_send(mq_clients[clients_count], (char*)&msg_send, sizeof(msg_send), 1);
                printf("server init client with id: %d\n", clients_count);
                clients_count++;

                break;
            case MSG:
                int id = 0; 
                while(id < clients_count){
                    if(msg_received.id != id){
                        mq_send(mq_clients[id], (char*)&msg_received, sizeof(msg_received), 1);
                        printf("server send message: %s from client with id: %d to client with id: %d\n", msg_received.msg, msg_received.id, id);
                    }
                    id++;
                }

                break;
            default:
                printf("unknow message type\n");
                break;
        }

    }

    return EXIT_SUCCESS;
}