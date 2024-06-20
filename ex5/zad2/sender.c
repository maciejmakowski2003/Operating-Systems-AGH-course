#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

void SIGUSR1_handler(int sig){
    printf("received confirmation\n");
}

int main(int argc, char** argv) {
    if(argc != 3){
        printf("expected: %s <catcher_pid> <new_status>", argv[0]);
        return EXIT_FAILURE;
    }

    long catcher_pid = atoi(argv[1]);
    int new_status = atoi(argv[2]);

    printf("sender pid: %d\n", getpid());

    signal(SIGUSR1,SIGUSR1_handler);

    union sigval sig_value = {new_status};
    sigqueue(catcher_pid,SIGUSR1,sig_value);
    printf("SIGUSR1 send to: %ld with new status: %d\n", catcher_pid, new_status);

    sigset_t sigset; 
    sigfillset(&sigset);
    sigdelset(&sigset, SIGUSR1);
    sigdelset(&sigset, SIGINT);
    sigsuspend(&sigset);

    return 0;
}
