#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <dlfcn.h>

int main (int l_param, char * wparam[]){
  int i;
  int tab[20]={1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0};
/*
1) otworz biblioteke
2) przypisz wskaznikom f1 i f2 adresy funkcji z biblioteki sumuj i srednia
3) stworz Makefile kompilujacy biblioteke 'bibl1' ladowana dynamicznie oraz kompilujacy ten program
4) Stosowne pliki powinny znajdowac sie w folderach '.', './bin', './'lib'. Nalezy uzyc: LD_LIBRARY_PATH
*/
  void* handle = dlopen("./libbibl1.so",RTLD_LAZY);
  int (* f1)(int* tab, int n) = dlsym(handle,"sumuj");
  double (* f2)(int a, int b) = dlsym(handle,"dziel");

  for (i=0; i<3; i++) printf("Wynik: %d, %lf\n", f1(tab+i, 20-i), f2(tab[i], tab[i+1]));

  dlclose(handle);
  return 0;
}
