#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include "args_spec.h"

#define REINDEERS_NUMBER 9
#define CYCLES_COUNT 4

pthread_t santa_thread;//one thread for santa
pthread_t reindeers_threads[REINDEERS_NUMBER];//one thread for each reindeer
reindeer_args_t reindeers_args[REINDEERS_NUMBER];//structure storing reindeer data

pthread_mutex_t santa_mutex;//one mutex for santa
pthread_mutex_t reindeers_mutex[REINDEERS_NUMBER];//one mutex for each reindeer
pthread_mutex_t reindeers_count_mutex;//mutex for incrementing number of returning reindeers

pthread_cond_t santa_cond = PTHREAD_COND_INITIALIZER;//all raindeers have to come back to wake up santa

int reindeers_count = 0;//number of reindeers that came back

void* santa_handler(void* arg);
void* reindeer_handler(void* arg);

void create_threads(){
    if(pthread_create(&santa_thread,NULL,santa_handler,NULL) != 0){
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }

    for(int i = 0; i < REINDEERS_NUMBER; i++){
        reindeers_args[i].id = i + 1;

        if(pthread_create(&reindeers_threads[i],NULL,reindeer_handler,&reindeers_args[i]) != 0){
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }
}

void initialize_mutexes() {
    if(pthread_mutex_init(&santa_mutex, NULL) != 0){
        perror("pthread_mutex_init");
            exit(EXIT_FAILURE);
    }

    if(pthread_mutex_init(&reindeers_count_mutex, NULL) != 0){
        perror("pthread_mutex_init");
            exit(EXIT_FAILURE);
    }

    for (int i = 0; i < REINDEERS_NUMBER; i++) {
        if (pthread_mutex_init(&reindeers_mutex[i], NULL) != 0) {
            perror("pthread_mutex_init");
            exit(EXIT_FAILURE);
        }
    }
}

void* santa_handler(void *arg){
    //4 cycles of delivering presents
    for(int i = 0; i < CYCLES_COUNT; i++){
        
        //wait until all reindeers come back
        pthread_mutex_lock(&santa_mutex);
        pthread_cond_wait(&santa_cond, &santa_mutex);
        pthread_mutex_unlock(&santa_mutex);

        //when all reindeers come back
        printf("Mikołaj: budzę się\n");
        printf("Mikołaj: dostarczam zabawki\n");

        sleep(rand()%3 + 2);
        printf("Mikołaj: zasypiam\n");

        //unlock reindeers
        for(int i = 0; i < REINDEERS_NUMBER; i++){
            pthread_mutex_unlock(&reindeers_mutex[i]);
        }
    }


    //cancel raindeers threads
    for(int i = 0; i < REINDEERS_NUMBER; i++){
        pthread_cancel(reindeers_threads[i]);
    }

    printf("Mikołaj: koniec tego dobrego\n");

    return NULL;
}

void* reindeer_handler(void *arg){
    reindeer_args_t* reindeer_arg = (reindeer_args_t*)arg;
    int id = reindeer_arg->id;

    //enable canceling reindeer thread immediately from other thread
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);

    //lock reindeer thread until it works for santa
    pthread_mutex_lock(&reindeers_mutex[id-1]);

    while(true){
        //reindeer's holidays
        sleep(rand()%6 + 5);

        //lock reindeer counter mutex to increment it
        pthread_mutex_lock(&reindeers_count_mutex);
        printf("Renifer: czeka %d reniferów, %d\n", reindeers_count, id);
        reindeers_count++;

        //if all raindeers come back, santa will be woken up
        if(reindeers_count == REINDEERS_NUMBER){
            printf("Renifer: wybudzam Mikołaja, %d\n", id);
            pthread_cond_signal(&santa_cond);
            reindeers_count = 0;
        }

        pthread_mutex_unlock(&reindeers_count_mutex);
        pthread_mutex_lock(&reindeers_mutex[id-1]);
    }
    return NULL;
}

int main() {
    srand(time(NULL));

    initialize_mutexes();
    create_threads();

    if (pthread_join(santa_thread, NULL) != 0) {
        perror("pthread_join");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < REINDEERS_NUMBER; i++) {
        if (pthread_join(reindeers_threads[i], NULL) != 0) {
            perror("pthread_join");
            exit(EXIT_FAILURE);
        }
    }

    return EXIT_SUCCESS;
}