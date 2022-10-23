#pragma once

typedef struct node Node;
   
struct node{
    int symbol;
    int count;
    Node *left;
    Node *right;
};

typedef struct branch Branch;

struct branch
{
    char s[10];
    Branch *prev; // 前の枝を指すポイント
};

// ファイルをエンコードし木のrootへのポインタを返す
Node *encode(const char *filename);
// Treeを走査して表示する
void traverse_tree(const int depth, const Node *root);

// Treeをツリー状に表示する
void print_tree(Node* node, Branch *branch, int position);
