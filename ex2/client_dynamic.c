#include <dlfcn.h>
#include <stdio.h>

int main(){
    int (*test_collatz_convergence)(int input, int max_iter);
    
    void *handle = dlopen("collatz/build/libcollatz_shared.so", RTLD_LAZY);
    if(!handle){
        fprintf(stderr, "%s\n", dlerror());
    }

    test_collatz_convergence = dlsym(handle,"test_collatz_convergence");    
    printf("%d\n", test_collatz_convergence(11, 30));
    printf("%d\n", test_collatz_convergence(10, 30));

    dlclose(handle);
}