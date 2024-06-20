#ifndef THREAD_SPEC_H
#define THREAD_SPEC_H

typedef struct {
	char** foreground; 
	char** background; 
	int first_cell;
	int last_cell;
} thread_args_t;

#endif //THREAD_SPEC_H