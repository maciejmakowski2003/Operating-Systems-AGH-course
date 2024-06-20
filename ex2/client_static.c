#include "collatz/collatz.h"
#include "stdio.h" 

int main(){
    printf("%d\n", test_collatz_convergence(8, 30));
    printf("%d\n", test_collatz_convergence(11, 30));
}