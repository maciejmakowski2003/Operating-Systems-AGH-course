#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

volatile int status = 0;
volatile int status_changes = 0;

void handle_status_change(int new_status){
    status_changes++;
    status = new_status;
}

void SIGUSR1_handler(int sig, siginfo_t *info, void *ucontext){
    handle_status_change(info->si_int);
    printf("from pid: %d recieved new status: %d\n", info->si_pid, info->si_int);
    kill(info->si_pid, SIGUSR1);
}

int main(){
    printf("catcher pid: %d\n", getpid());

    struct sigaction act; 
    act.sa_flags = SA_SIGINFO;
    act.sa_sigaction = SIGUSR1_handler; 
    sigemptyset(&act.sa_mask);  
    sigaction(SIGUSR1,&act,NULL);

    while(1){
        switch(status){
            case 1:
                for(int i = 1; i <= 100; i++){
                    printf("%d, ", i);
                }
                printf("\n");
                status = 0;
                break; 
            case 2:
                printf("number of status changes: %d\n", status_changes);
                status = 0;
                break;
            case 3:
                printf("exit\n"); 
                exit(EXIT_SUCCESS);
            default:
                break;
        }
    }

    return 0;
}