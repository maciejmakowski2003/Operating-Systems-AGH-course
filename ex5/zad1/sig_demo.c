#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>


void SIGUSR1_handler(int sig){
    printf("SIGUSR1 is received with signal number: %d\n", sig);
}

int main(int argc, char** argv) {
    if(argc != 2) {
        printf("Expected: %s <disposition>",argv[0]);
        return EXIT_FAILURE;
    }
    char* disposition = argv[1];

    if(!strcmp("none", disposition)) {
        if (signal(SIGUSR1, SIG_DFL) == SIG_ERR) {
            perror("Error setting SIGUSR1 to default handler");
            exit(EXIT_FAILURE);
        }
        raise(SIGUSR1);
    }

    else if(!strcmp("ignore", disposition)) {
        if (signal(SIGUSR1, SIG_IGN) == SIG_ERR) {
            perror("error during setting SIGUSR1 to SIG_IGN");
            exit(EXIT_FAILURE);
        }
        raise(SIGUSR1);
    }

    else if(!strcmp("handler", disposition)) {
        if (signal(SIGUSR1, SIGUSR1_handler) == SIG_ERR) {
            perror("error during setting SIGUSR1 handler");
            exit(EXIT_FAILURE);
        }
        raise(SIGUSR1);
    }

    else if(!strcmp("mask", disposition)) {
        sigset_t sigset;
        sigemptyset(&sigset);
        sigaddset(&sigset,SIGUSR1);

        if(sigprocmask(SIG_SETMASK,&sigset,NULL) < 0) {
            perror("error during setting new signal mask");
            exit(EXIT_FAILURE);
        }
        raise(SIGUSR1);

        sigset_t pending_sig; 
        if(sigpending(&pending_sig) < 0) {
            perror("error during fetching all pending signals");
            exit(EXIT_FAILURE);
        }

        char* is_member; 
        switch(sigismember(&pending_sig,SIGUSR1)) {
            case 1:
                is_member = "yes";
                break;
            case -1:
                is_member = "no";
                break;
            default:
                is_member = "error during checking";
        }
        printf("Is SIGUSR1 a member of pending signals? : %s\n", is_member);
    }

    else{
        perror("wrong disposition. available dispositions: none, ignore, handler, mask");
        exit(EXIT_FAILURE);
    }

    return 0;
}