#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <math.h>


double left_border = 0.0;
double right_border = 1.0;
double f(double x){ return 4/(x*x + 1);}

double min(double x, double y){ return (x>y) ? y : x;}

double integrate(double (*f)(double),double start, double end, double interval_width){
    double result = 0.0; 

    while(start<end){
        double mid = (start+(start+interval_width))/2.0;
        result += f(mid)*min(end-start,interval_width);
        start += interval_width;
    }

    return result;    
}

int main(int argc, char** argv){
    if(argc != 3){
        printf("expected: %s <integral width> <number of processes>\n", argv[0]);
        return EXIT_FAILURE;
    }

    double interval_width = strtod(argv[1],NULL);
    int processes_number = atoi(argv[2]);
    int intervals_per_process = ceil((right_border-left_border)/interval_width)/processes_number;

    double start = left_border;
    double end = left_border + intervals_per_process*interval_width;
    
    int fd[processes_number][2];

    for(int i = 0; i < processes_number; i++){
        end = min(end,right_border);

        if(pipe(fd[i])<0){
            printf("error during creating pipe\n");
            return EXIT_FAILURE;
        }

        pid_t child = fork();

        if(child == 0){
            close(fd[i][0]);

            double partial_result = integrate(f,start,end,interval_width);

            if(write(fd[i][1],&partial_result,sizeof(partial_result))<0){
                printf("error during writing to pipe\n");
                return EXIT_FAILURE;
            }

            exit(0);
        } else{
            close(fd[i][1]);
        }

        start = end;
        end = start + intervals_per_process*interval_width;
    }  

    double result = 0.0;

    for(int i = 0; i<processes_number; i++){
        double partial_result;

        if(read(fd[i][0],&partial_result,sizeof(partial_result))<0){
            printf("error during reading from pipe\n");
            return EXIT_FAILURE;
        }

        result += partial_result;
    }

    printf("integration result: %lf\n",result);
    return EXIT_SUCCESS;
}