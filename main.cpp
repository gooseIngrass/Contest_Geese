#include <stdio.h>
#include <vector>
#include <map>
#include "huffman.cpp"

int main(){

    string test = "33ABCDEABCD ABCDEABCD";
    vector<char> huffTest = {test.begin(), test.end()};
    vector<int> convetedInput = convertUsingTables(huffTest);
    distanceCodeTable();
//    map<char, int> freq = countFreqs(huffTest);
//    vector<Node*> tree = createLeafs(freq);
    map<char, string>huffCodes = genCodes(huffTest);
//    vector<char>encodedData = encode(huffCodes, huffTest, tree);
    for(auto& i : huffCodes){
        printf("%c : %s\n", i.first, i.second.c_str());
    }
    return 0;
}