#include <stdio.h>

#if defined(BYTE)

void reverse(FILE* input_file, FILE* output_file){
    //move to the last character in file
    fseek(input_file,-1,SEEK_END);
    //get the length of file
    size_t size = ftell(input_file) + 1;
    while(size>0) {
        //write formatted output to stream
        fprintf(output_file,"%c",getc(input_file));
        //-2 because getc() moves pointer to the next byte, so we have to move
        //two steps backward
        fseek(input_file,-2,SEEK_CUR);
        size--;
    }
}

#elif !defined(BYTE)

#define BUFFER_SIZE 1024

void reverse_buffer(char* buffer, size_t size){
    char tmp; 

    for(int i = 0, j = size -1; i < j; i++, j--){
        tmp = buffer[i];
        buffer[i] = buffer[j];
        buffer[j] = tmp;
    }
}

void reverse(FILE* input_file, FILE* output_file){
    fseek(input_file,0,SEEK_END);
    char buffer[BUFFER_SIZE];
    size_t size = ftell(input_file); 

    while(size > 0) {
        //count how many bytes left to read, if less then BUFFER_SIZE, it will modify number of bytes to read
        size_t to_read = (size>=BUFFER_SIZE) ? BUFFER_SIZE : size; 
        //move file pointer backward by to_read bytes
        fseek(input_file,-to_read,SEEK_CUR);
        //copy to_read elements to buffer
        size_t bytes_read = fread(buffer,sizeof(char),to_read,input_file);
        //reverse buffer 
        reverse_buffer(buffer,bytes_read);
        //write to output_file
        fwrite(buffer,sizeof(char),bytes_read,output_file);
        //move pointer backwards by to_read bytes
        fseek(input_file,-bytes_read,SEEK_CUR);
        size-=to_read;
    }

}

#endif


int main(int argc, char **argv){

    if (argc != 3){
        printf("You have to pass 3 args");
        return -1;
    }

    FILE* input_file = fopen(argv[1],"r"); 

    if(input_file == NULL){
        printf("Error during opening input file");
        return -1;
    }

    FILE* output_file = fopen(argv[2],"w"); 

    if(output_file == NULL){
        printf("Error during opening output file");
        return -1;
    }

    reverse(input_file,output_file);

    fclose(input_file);
    fclose(output_file);

    return 0;
}