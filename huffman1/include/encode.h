#pragma once

typedef struct node Node;
   
struct node{
    int symbol;
    int count;
    Node *left;
    Node *right;
};

// symbolのcodeを保持する構造体
typedef struct code Code;

struct  code{
    int symbol;
    int bits_len;
    int bits;
    Code *next;
};

// ファイルをエンコードし木のrootへのポインタを返す
Node *encode(const char *filename);

// Treeを走査して表示する
void traverse_tree(const int depth, const Node *root);

