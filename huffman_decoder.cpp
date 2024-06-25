
int fileDecoder(char *inputFN, char *outFN){
    FILE *input = fopen(inputFN, "rb");
    if(!input) return 404;

    FILE *output = fopen(outFN, "w");
    if(!output) return 404;

    vector<Node*> tree;
    int treeSize;
    fread(&treeSize, sizeof(treeSize), 1, input); //Считываем размер дерева из файла

    for(int i = 0; i < treeSize; i++){                                         //Считываем само дерево из файла
        Node *n;
        fread(&n, sizeof(n), 1, input);
        tree.push_back(n);
    }

    int count = 0;
    char byte;

    Node *treeP = tree[0];

    while(fread(&byte, 1, 1, input) == 1){ //Проходим по считаному байту и восстанавливаем
        bool b = byte & (1 << 7-count);                                //сообщение

        if(treeP->left == nullptr && treeP->right == nullptr){
            fputc(treeP->data, output);
            treeP = tree[0];
        }
        if(b){
            treeP = treeP->right;
        }
        else{
            treeP = treeP->left;
        }

        count++;
        if(count == 8) {
            count = 0;
            byte = 0;
        }

    }
    fputs("Here will be your decoded data", output);
    fclose(input);
    fclose(output);
    return 1;
}
