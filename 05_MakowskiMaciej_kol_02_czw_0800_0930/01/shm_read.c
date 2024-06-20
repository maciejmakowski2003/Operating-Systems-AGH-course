#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <string.h>
#include <errno.h>

#define PATH "/tmp"
#define BUFFER_SIZE 1024
#define ID 0

int main(int argc, char const *argv[])
{
    char *shmAddr;

    // przekształć ścieżkę i identyfikator na klucz komunikacji międzyprocesowej (IPC) Systemu V
    key_t key = ftok(PATH, ID);
    if (key == -1) {
        fprintf(stderr, "ftok error: %s\n", strerror(errno));
        exit(1);
    }

    // Utwórz segment pamięci współdzielonej
    // zwracaną wartością jest identyfikator bloku pamięci współdzielonej - czyli shmid
    int shmid;
    if ((shmid = shmget(key, BUFFER_SIZE, 0666)) == -1) {
        fprintf(stderr, "shmget error: %s\n", strerror(errno));
        exit(1);
    }

    // Mapuj segmenty pamięci współdzielonej na przestrzeń adresową procesu
    shmAddr = (char *)shmat(shmid, NULL, 0);
    if (shmAddr == (char *)(-1)) {
        fprintf(stderr, "shmat error: %s\n", strerror(errno));
        exit(1);
    }

    printf("%s\n", shmAddr);

    // Rozłącz
    if (shmdt(shmAddr) == -1) {
        fprintf(stderr, "shmdt error: %s\n", strerror(errno));
        exit(1);
    }

    // Usuń segment pamięci współdzielonej
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        fprintf(stderr, "shmctl error: %s\n", strerror(errno));
        exit(1);
    }

    return 0;
}

/*
gcc -o shm_read shm_read.c
*/
