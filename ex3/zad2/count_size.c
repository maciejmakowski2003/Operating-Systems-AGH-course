#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>

int main(int argc, char **argv){
    DIR* dir = opendir(".");
    if( dir == NULL){
        perror("Error during opening current directory");
        return EXIT_FAILURE;
    }

    struct dirent *dir_file;
    struct stat stat_buffer;
    long long size = 0;

    while((dir_file = readdir(dir)) != NULL){
        stat(dir_file->d_name, &stat_buffer);

        if(!S_ISDIR(stat_buffer.st_mode)){
            printf("%lld %s\n", (long long) stat_buffer.st_size, dir_file->d_name);
            size+=stat_buffer.st_size;
        }
    }
    printf("%lld\n", size);
    closedir(dir);
    return EXIT_SUCCESS;
}