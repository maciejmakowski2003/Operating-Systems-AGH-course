#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <sys/mman.h>
#include <errno.h>
#include <string.h>
#include "shared_memory.h"

// function to handle errors and exit
void handle_error(const char* message) {
    perror(message);
    exit(EXIT_FAILURE);
}

// function to create and initialize shared memory region
printers_mmap_t* create_shared_memory(const char* name, size_t size) {
    int shared_mem_fd = shm_open(name, O_CREAT | O_RDWR, 0600);
    if (shared_mem_fd < 0)
        handle_error("shm_open");

    if (ftruncate(shared_mem_fd, size) < 0)
        handle_error("ftruncate");

    printers_mmap_t* printers_mmap = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, shared_mem_fd, 0);
    if (printers_mmap == MAP_FAILED)
        handle_error("mmap");

    return printers_mmap;
}

// function to initialize unnamed semaphores for printers and their states
void initalize_printers_attributes(printers_mmap_t* printers_mmap, int printers_number) {
    for (int i = 0; i < printers_number; i++) {
        if (sem_init(&printers_mmap->printers[i].printer_sem, 1, 1) < 0)
            handle_error("sem_init");
        printers_mmap->printers[i].state = WAITING;
    }
}

// function for printer process
void printer_process(printers_mmap_t* printers_mmap, int printer_index) {
    while (true) {
        if (printers_mmap->printers[printer_index].state == WORKING) {
            for (int j = 0; j < printers_mmap->printers[printer_index].printer_buffer_size; j++) {
                printf("%c", printers_mmap->printers[printer_index].printer_buffer[j]);
                sleep(1);
            }
            printf("\n");
            fflush(stdout);

            printers_mmap->printers[printer_index].state = WAITING;

            if (sem_post(&printers_mmap->printers[printer_index].printer_sem) < 0)
                handle_error("sem_post");
        }
    }
    exit(EXIT_SUCCESS);
}

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("usage: %s <printers number>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int printers_number = atoi(argv[1]);

    if (printers_number > MAX_PRINTERS_NUMBER || printers_number < 1) {
        printf("printers number have to be between 1 and %d\n", MAX_PRINTERS_NUMBER);
        return EXIT_FAILURE;
    }

    printers_mmap_t* printers_mmap = create_shared_memory(PRINTERS_SHARED_MEMORY_NAME, sizeof(printers_mmap_t));
    memset(printers_mmap, 0, sizeof(printers_mmap_t));
    printers_mmap->printers_number = printers_number;
    initalize_printers_attributes(printers_mmap, printers_number);

    for (int i = 0; i < printers_number; i++) {
        pid_t printer_pid = fork();
        if (printer_pid < 0)
            handle_error("fork");
        else if (printer_pid == 0)
            printer_process(printers_mmap, i);
    }

    // wait for all child processes to terminate
    while (wait(NULL) > 0);

    // destroy semaphores
    for (int i = 0; i < printers_number; i++) {
        if (sem_destroy(&printers_mmap->printers[i].printer_sem) < 0)
            handle_error("sem_destroy");
    }

    // unmap shared memory
    if (munmap(printers_mmap, sizeof(printers_mmap_t)) < 0)
        handle_error("munmap");

    // unlink shared memory region
    if (shm_unlink(PRINTERS_SHARED_MEMORY_NAME) < 0)
        handle_error("shm_unlink");

    return EXIT_SUCCESS;
}
