#include "huffman.h"
#include "bitstream.h"

map<int, pair<int, int>>lengthCodeTable(){
    vector<vector<int>> lengthCodeRanges = {//{код символа, сдвиг, нижняя граница, верхняя граница}
            {257,0,3,3}, {258,0,4,4}, {259,0,5,5}, {260,0,6,6},
            {261,0,7,7},{262,0,8,8},{263,0,9,9},{264,0,10,10},
            {265,1,11,12},{266,1,13,14},{267,1,15,16},{268,1,17,18},
            {269,2,19,22},{270,2,23,26},{271,2,27,30},{272,2,31,34},
            {273,3,35,42},{274,3,43,50},{275,3,51,58},{276,3,59,66},
            {277,4,67,82},{278,4,83,98},{279,4,99,114},{280,4,115,130},
            {281,5,131,162},{282,5,163,194},{283,5,195,226},{284,5,227,257},{285,0,258,258}
    };

    //Составляем таблицу кодов длин
    map<int, pair<int, int>> lengthCodes;
    for (const auto& range : lengthCodeRanges) {
        int code = range[0];
        int offset = range[1];
        int lower_bound = range[2];
        int upper_bound = range[3];
        for (int i = lower_bound; i <= upper_bound; i++) {
            lengthCodes[code] = {offset, lower_bound};
        }
    }
    return lengthCodes;
}



map<int, pair<int, int>>distanceCodeTable(){
    int distanceCodeRanges[30][4] = {//{код символа, сдвиг, нижняя граница, верхняя граница}
            {0, 0, 1, 1},
            {1, 0, 2, 2},
            {2, 0, 3, 3},
            {3, 0, 4, 4},
            {4, 1, 5, 6},
            {5, 1, 7, 8},
            {6, 2, 9, 12},
            {7, 2, 13, 16},
            {8, 3, 17, 24},
            {9, 3, 25, 32},
            {10, 4, 33, 48},
            {11, 4, 49, 64},
            {12, 5, 65, 96},
            {13, 5, 97, 128},
            {14, 6, 129, 192},
            {15, 6, 193, 256},
            {16, 7, 257, 384},
            {17, 7, 385, 512},
            {18, 8, 513, 768},
            {19, 8, 769, 1024},
            {20, 9, 1025, 1536},
            {21, 9, 1537, 2048},
            {22, 10, 2049, 3072},
            {23, 10, 3073, 4096},
            {24, 11, 4097, 6144},
            {25, 11, 6145, 8192},
            {26, 12, 8193, 12288},
            {27, 12, 12289, 16384},
            {28, 13, 16385, 24576},
            {29, 13, 24577, 32768}
    };

    map<int, pair<int, int>> distanceCodes;
    for (const auto& range : distanceCodeRanges) {
        int code = range[0];
        int offset = range[1];
        int lower_bound = range[2];
        int upper_bound = range[3];
        for (int i = lower_bound; i <= upper_bound; i++) {
            distanceCodes[code] = {offset, lower_bound};
        }
    }
    return distanceCodes;
};

vector<int> convertUsingTables(vector<char>& input){
    vector<int> convertedInput;

    for(char ch : input){
        convertedInput.push_back(ch);
    }
    return convertedInput;
    }


map<char, int> countFreqs(FILE *input){//Считаем сколько раз каждый символ встречается в инпуте
    map<char, int> freq;
    char c;

    while(fread(&c, 1, 1, input) == 1){
        ++freq[c];
    }
    return freq;

}

Node* createNode(char data, int freq, Node* left, Node* right){
    Node* node = new Node();

    node->data = data;
    node->freq = freq;
    node->left = left;
    node->right = right;

    return node;

}

vector<Node*> createLeafs(const map<char, int>& freqs){//На основе инпута создаем листья(символ и число его появлений в инпуте)
    vector<Node*> tree;

    for(const auto& [ch, fr] : freqs){
        if (fr > 0){
            tree.push_back(createNode(ch, fr, nullptr, nullptr));

        }
    }

    return tree;

}

bool comparator(Node* a, Node* b){
    return a->freq < b->freq;

}

Node* buildTree(vector<Node*>& tree){
    sort(tree.begin(), tree.end(), comparator);


    while(tree.size() > 1){
        //Берем два наименьших узла из отсортированного дерева

        Node* left = tree[0];
        Node* right = tree[1];

        //Создаем внутренний узел с двумя наследниками и суммой частоты их появления
        int sum = left->freq + right->freq;
        Node* parent = createNode('\0', sum, left, right);

        //Добавляем созданный узел в дерево и стираем его наследников
        tree.push_back(parent);
        tree.erase(tree.begin(), tree.begin() + 2);
    }

    return tree[0]; //возвращаем корень дерева
}

void encodeNode(Node* node, vector<bool> code,  map<char, vector<bool>>& huffCodes){
    if(node->left == nullptr && node->right == nullptr){
        huffCodes[node->data] = code;
    }

    else{       
        code.push_back(0);
        encodeNode(node->left, code, huffCodes);

        code.pop_back();
        
        code.push_back(1);
        encodeNode(node->right, code, huffCodes);
    }
}


map<char, vector<bool>> genCodes(FILE *input){
    map<char, int> freq = countFreqs(input);
    vector<Node*> tree = createLeafs(freq);

    Node* root = buildTree(tree);
    map<char, vector<bool>> huffTable;

    vector<bool> code;
    encodeNode(root, code, huffTable);      

    return huffTable;
}

int codesToOutput(map<char, vector<bool>> huffTable, FILE *input, FILE *output){
    int count = 0;
    char buf;
    vector<bool>code;

    rewind(input);
    char c;

    while(fread(&c, 1, 1, input) == 1){
        code = huffTable[c];
        
        for(int i = 0; i < code.size(); i++){
            buf = buf | code[i] << (7 - count);
            count++;

            if(count == 8) {
                printf("%c\n", buf);
                count = 0;
                fwrite(&buf, sizeof(char), 1, output);
                buf = 0;
            }
        }  

        // for(bool b : code){
        //     printf("%d", b);
        // }
    }
    return 1;

}





