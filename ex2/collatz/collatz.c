#include "collatz.h"

int collatz_conjecture(int input){
    return input%2 == 0 ? input/2 : 3 * input + 1;
}

int test_collatz_convergence(int input, int max_iter){
    int i = 0; 

    while(input != 1 || i <= max_iter){
        input = collatz_conjecture(input);
        i+=1;
    }

    return input == 1 ? i : -1;
}