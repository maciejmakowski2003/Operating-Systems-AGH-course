#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <math.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "pipes.h"

int main(int argc, char** argv){
    if(argc != 4){
        printf("expected: %s <interval_start> <interval_end> <interval_width>\n",argv[0]);
        return EXIT_FAILURE;
    }
    range_info range; 
    range.start = strtod(argv[1],NULL);
    range.end = strtod(argv[2],NULL);
    range.interval_width = strtod(argv[3],NULL);

    if(range.interval_width<=0){
        printf("interval width should be positive number\n");
    }

    int input_pipe = open(INPUT_PIPE_NAME,O_WRONLY);
    if(input_pipe<0){
        printf("error opening input pipe\n");
        return EXIT_FAILURE;
    }

    int output_pipe = open(OUTPUT_PIPE_NAME,O_RDONLY);
    if(output_pipe<0){
        printf("error opening output pipe\n");
        return EXIT_FAILURE;
    }

    if(write(input_pipe,&range,sizeof(range))<0){
        printf("error writing into input pipe\n");
            return EXIT_FAILURE;
    }

    double result;

    if(read(output_pipe,&result,sizeof(result))<0){
        printf("error reading from output pipe\n");
        return EXIT_FAILURE;
    }

    printf("integration result: %f\n", result);

    close(input_pipe);
    close(output_pipe);

    return EXIT_SUCCESS;
}