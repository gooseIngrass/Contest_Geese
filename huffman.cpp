#include "huffman.h"

map<char, int> countFreqs(FILE *input){ //Считаем сколько раз каждый символ встречается в инпуте
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

void encodeNode(Node* node, vector<bool> code,  map<char, vector<bool>>& huffCodes){ //Проходя по дереву формируем коды для каждого символа
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

int codesToOutput( vector<Node*> tree, map<char, vector<bool>> huffTable, FILE *input, FILE *output){ //Переписываем исходный файл в соответствии с таблицей кодов
    int count = 0;
    char buf, c;
    vector<bool>code;

    rewind(input);

    int treeSize = tree.size();
    fwrite(&treeSize, sizeof(treeSize), 1, output); //Записываем размер дерева

    for(auto i : tree){
        fwrite(&i, sizeof(i), 1, output); // Записываем дерево в файл для декодера
    }

    while(fread(&c, 1, 1, input) == 1){ //Считываем посимвольно
        code = huffTable[c]; //Получаем код символа

        for(int i = 0; i < code.size(); i++){  //Упаковываем код в байт и записываем его в файл
            buf = buf | code[i] << (7 - count);
            count++;

            if(count == 8) {
                count = 0;
                fwrite(&buf, sizeof(char), 1, output);
                buf = 0;
            }
        }
    }
    return 1;

}


int writeGenCodes(char *inputFN, char *outFN){ //Создаём таблицу кодов Хаффмана
    FILE *input = fopen(inputFN, "rb");
    if (!input) {
        printf("File doesnt exist");
        return 404;
    }

    FILE *output = fopen(outFN, "wb");
    if(!output){
        printf("File doesnt exist");
        return 404;
    }

    map<char, int> freq = countFreqs(input);
    vector<Node*> tree = createLeafs(freq);

    Node* root = buildTree(tree);
    map<char, vector<bool>> huffTable;

    vector<bool> code;
    encodeNode(root, code, huffTable);

    codesToOutput(tree, huffTable, input, output);

    fclose(input);
    fclose(output);
    return 1;
}







