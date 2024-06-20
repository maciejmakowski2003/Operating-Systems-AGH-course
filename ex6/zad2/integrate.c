#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <math.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "pipes.h"

double min(double x, double y){ return (x>y) ? y : x;}

double f(double x){ return 4/(x*x + 1);}

double integrate(double (*f)(double),double start, double end, double interval_width){
    double result = 0.0; 

    while(start<end){
        double mid = (start+(start+interval_width))/2.0;
        result += f(mid)*min(end-start,interval_width);
        start += interval_width;
    }

    return result;    
}

int main(){
    if(mkfifo(INPUT_PIPE_NAME, S_IRWXU) != 0){
        printf("error creating input pipe\n");
        return EXIT_FAILURE;
    }

    if(mkfifo(OUTPUT_PIPE_NAME, S_IRWXU) != 0){
        printf("error creating output pipe\n");
        return EXIT_FAILURE;
    }

    int input_pipe = open(INPUT_PIPE_NAME,O_RDONLY);
    if(input_pipe<0){
        printf("error opening input pipe\n");
        return EXIT_FAILURE;
    }

    int output_pipe = open(OUTPUT_PIPE_NAME,O_WRONLY);
    if(output_pipe<0){
        printf("error opening output pipe\n");
        return EXIT_FAILURE;
    }

    range_info range;
    double result;

    while(read(input_pipe,&range,sizeof(range))>0){
        result = integrate(f,range.start,range.end,range.interval_width);
        if(write(output_pipe,&result,sizeof(result))<0){
            printf("error writing into output pipe\n");
            return EXIT_FAILURE;
        }
    }

    close(input_pipe);
    close(output_pipe);    

    return EXIT_SUCCESS;        
}