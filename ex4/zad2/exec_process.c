#include <sys/resource.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int global = 0;

int main(int argc, char** argv) {     
    if(argc !=  2) {
        printf("input format: <dir_path>");
        return -1;
    }

    printf("%s \n",argv[0]);

    int local = 0;

    pid_t child = fork();

    if(child == 0){
        printf("child process\n");
        global++;
        local++;
        printf("child pid = %d, parent pid = %d\n", getpid(), getppid());
        printf("child's local = %d, child's global = %d\n", local, global);
        int status = execl("/bin/ls", "ls","-l", argv[1], NULL);
        exit(status);
    }

    int status;
    waitpid(child, &status, 0);
    int child_exit_code = WEXITSTATUS(status);

    printf("parent process\n");
    printf("parent pid = %d, child pid = %d\n", getppid(), getpid());
    printf("child exit code: %d\n", child_exit_code);
    printf("parent's local = %d, parent's global = %d\n", local, global);
    
    return child_exit_code;
}