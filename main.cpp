#include <stdio.h>
#include <vector>
#include <map>
#include "huffman.cpp"

int main(){

    string test = "33ABCDEABCD ABCDEABCD";
    FILE *input = fopen("test_input.txt", "rb");
    if (!input) return 1;

    map<char, vector<bool>> huffCodes = genCodes(input);

    // for(const auto& [c, code] : huffCodes){
    //     printf("%c : ", c);
    //     for(bool b : code){
    //         printf("%d", b);
    //     }
    //     printf("\n");
    // }

    FILE *output = fopen("test_output.bin", "wb");
    if(!output) return 1;


    codesToOutput(huffCodes, input, output);

    fclose(input);
    fclose(output);
    return 0;
}