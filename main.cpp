#include <stdio.h>
#include <vector>
#include <filesystem>
#include <map>
#include <string.h>
#include "huffman.cpp"
#include "huffman_decoder.cpp"


int main(int argc, char *argv[]){
    writeGenCodes(argv[1], "archived.bin");
//    fileDecoder("archived.bin", "decoded.txt");

    return 0;
}