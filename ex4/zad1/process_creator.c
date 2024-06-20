#include <sys/resource.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char** argv) {     
    if(argc !=  2) {
        printf("input format: <number of processes to create>");
        return -1;
    }

    int number_of_processes = atoi(argv[1]);

    for(int i = 0; i < number_of_processes; i++) {
        pid_t pid = fork();

        if(pid == 0) {
            printf("Parent process pid: %d Child process pid %d\n", getppid(), getpid());
            exit(0);
        }
    }

    while(wait(NULL) > 0);
    printf("%d processes were created!\n", number_of_processes);
    
    return 0;
}