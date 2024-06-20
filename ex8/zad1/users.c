#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <sys/mman.h>
#include <errno.h>
#include <time.h>
#include "shared_memory.h"

// function to handle errors and exit
void handle_error(const char* message) {
    perror(message);
    exit(EXIT_FAILURE);
}

// function to open and map shared memory region
printers_mmap_t* open_shared_memory(const char* name, size_t size) {
    int shared_mem_fd = shm_open(name, O_RDWR, 0600);
    if (shared_mem_fd < 0)
        handle_error("shm_open");

    printers_mmap_t* printers_mmap = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, shared_mem_fd, 0);
    if (printers_mmap == MAP_FAILED)
        handle_error("mmap");

    return printers_mmap;
}

//function to generate random string from a to z
char* generate_random_string(int length, int user_id) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyz";
    int charset_size = sizeof(charset) - 1;
    time_t t;

    char* random_string = malloc((length + 1) * sizeof(char));
    srand((unsigned) time(&t) * user_id);

    for (int i = 0; i < length; i++) {
        random_string[i] = charset[rand() % charset_size];
    }
    random_string[length] = '\0'; 

    return random_string;
}

// function to find free printer, if there is not one, function returns random
int find_free_printer(printers_mmap_t* printers_mmap){
    for(int i = 0; i<printers_mmap->printers_number; i++){
        int valp; 
        sem_getvalue(&printers_mmap->printers[i].printer_sem, &valp);
        if(valp > 0){
            return i;
        }
    }

    return rand() % printers_mmap->printers_number;
}

// function to order printing
void place_task(printers_mmap_t* printers_mmap, int i){
    while(true){
        int printer_number = find_free_printer(printers_mmap);
        char* random_string = generate_random_string(10, i);

        if(sem_wait(&printers_mmap->printers[printer_number].printer_sem) < 0)
            handle_error("sem_wait");

        
        strncpy(printers_mmap->printers[printer_number].printer_buffer, random_string, MAX_BUFFER_SIZE);
        printers_mmap->printers[printer_number].printer_buffer_size = strlen(random_string);

        printf("user: %d print: %s on printer: %d\n", i, random_string, printer_number);
        free(random_string);
        printers_mmap->printers[printer_number].state = WORKING;

        sleep(1 + rand()%4);
    }
    exit(EXIT_SUCCESS);
}

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("usage: %s <users number>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int users_number = atoi(argv[1]);

    if (users_number > MAX_USERS_NUMBER || users_number < 1) {
        printf("users number have to be between 1 and %d\n", MAX_USERS_NUMBER);
        return EXIT_FAILURE;
    }

    printers_mmap_t* printers_mmap = open_shared_memory(PRINTERS_SHARED_MEMORY_NAME, sizeof(printers_mmap_t));

    for(int i = 0; i < users_number; i++){
        pid_t user_pid = fork(); 
        if (user_pid < 0) 
            handle_error("fork");
        else if(user_pid == 0) {
            place_task(printers_mmap, i);
        }
    }

    // wait for all child processes to terminate
    while (wait(NULL) > 0);

    // unmap shared memory
    if (munmap(printers_mmap, sizeof(printers_mmap_t)) < 0)
        handle_error("munmap");
}