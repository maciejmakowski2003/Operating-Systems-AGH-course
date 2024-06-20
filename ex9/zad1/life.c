#define _XOPEN_SOURCE 700
#define _DEFAULT_SOURCE
#include <ncurses.h>
#include <locale.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <math.h>
#include <signal.h>
#include "grid.h"
#include "thread_spec.h"

#define MIN(a, b) (((a) < (b)) ? (a) : (b))

void* thread_work(void *arg) {
    thread_args_t* thread_args = (thread_args_t*) arg;

    while (true) {
        pause();

        for (int i = thread_args->first_cell; i <= thread_args->last_cell; i++) {
            int row = i / GRID_WIDTH;
            int column = i % GRID_WIDTH;
            (*thread_args->background)[i] = is_alive(row, column, *thread_args->foreground);
        }
    }
    return NULL;
}

void wake_up_handler(int signo) {
    // Empty signal handler to wake up threads
}

void setup_signal_handler() {
    struct sigaction sa;
    sa.sa_handler = wake_up_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }
}

int validate_input(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <number_of_threads>\n", argv[0]);
        return -1;
    }

    int threads_number = atoi(argv[1]);
    if (threads_number <= 0 || threads_number > GRID_WIDTH * GRID_HEIGHT) {
        fprintf(stderr, "Number of threads is too large or invalid\n");
        return -1;
    }

    return threads_number;
}

void create_threads(int threads_number, pthread_t* threads, thread_args_t* thread_args, char** foreground, char** background) {
    int cells_per_thread = ceil(GRID_WIDTH * GRID_HEIGHT / threads_number);

    for (int i = 0; i < threads_number; i++) {
        thread_args[i].first_cell = i * cells_per_thread;
        thread_args[i].last_cell = MIN((i + 1) * cells_per_thread, GRID_WIDTH * GRID_HEIGHT) - 1;
        thread_args[i].foreground = foreground;
        thread_args[i].background = background;

        if (pthread_create(&threads[i], NULL, thread_work, &thread_args[i]) != 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }
}

void cleanup(char* foreground, char* background, pthread_t* threads, int threads_number) {
    for (int i = 0; i < threads_number; i++) {
        pthread_cancel(threads[i]);
        pthread_join(threads[i], NULL);
    }

    destroy_grid(foreground);
    destroy_grid(background);
    endwin();
}

int main(int argc, char** argv) {
    int threads_number = validate_input(argc, argv);
    if (threads_number == -1) {
        return EXIT_FAILURE;
    }

    setup_signal_handler();

    srand(time(NULL));
    setlocale(LC_CTYPE, "");
    initscr(); // Start curses mode

    char *foreground = create_grid();
    char *background = create_grid();
    char *tmp;

    init_grid(foreground);

    pthread_t threads[threads_number];
    thread_args_t thread_args[threads_number];

    create_threads(threads_number, threads, thread_args, &foreground, &background);

    while (true) {
        draw_grid(foreground);

        for (int i = 0; i < threads_number; i++) {
            pthread_kill(threads[i], SIGUSR1);
        }

        usleep(500 * 1000);

        update_grid(foreground, background);
        tmp = foreground;
        foreground = background;
        background = tmp;
    }

	//// End curses mode and clean up
    cleanup(foreground, background, threads, threads_number);

    return EXIT_SUCCESS;
}
