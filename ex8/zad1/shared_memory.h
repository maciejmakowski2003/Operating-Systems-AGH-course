#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include <semaphore.h>

#define MAX_PRINTERS_NUMBER 16
#define MAX_USERS_NUMBER 16

#define PRINTERS_SHARED_MEMORY_NAME "printers_shared_memory"

#define MAX_BUFFER_SIZE 1024

typedef enum{
    WORKING,
    WAITING
} printer_state;

typedef struct{
    sem_t printer_sem; 
    size_t printer_buffer_size; 
    char printer_buffer[MAX_BUFFER_SIZE];
    printer_state state;
} printer_t;

typedef struct{
    printer_t printers[MAX_PRINTERS_NUMBER];
    int printers_number;
} printers_mmap_t;

#endif //SHARED_MEMORY_H