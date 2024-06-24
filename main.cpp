#include <stdio.h>
#include <vector>
#include <filesystem>
#include <map>
#include "huffman.cpp"

int main(int argc, char *argv[]){
    FILE *input = fopen(argv[1], "rb");
    if (!input) return 1;

    FILE *output = fopen("archived.bin", "wb");
    if(!output) return 1;

    map<char, vector<bool>> huffCodes = genCodes(input); //Генерируем таблицу кодов

    codesToOutput(huffCodes, input, output); //Кодируем файл

    fclose(input);
    fclose(output);
    return 0;
}