#include "huffman.cpp"
#include "huffman_decoder.cpp"
#include "main.h"

int main(int argc, char *argv[]){
    if(argc > 1){
        if(strcmp(argv[1], "-help") == 0){
            help();
        }

        if(strcmp(argv[1], "-c") == 0){
            writeGenCodes(argv[2], argv[3]);
        }

        if(strcmp(argv[1], "-e") == 0){
            fileDecoder(argv[2], argv[3]);
        }

    }else{
        printf("Specify the path to the file. \nFor reference, enter archiver -help");
    }

    return 0;
}