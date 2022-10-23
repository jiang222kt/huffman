#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include "encode.h"

#define NSYMBOLS 256
#define MAX_DEPTH 20

static int symbol_count[NSYMBOLS];

// ツリー上での各symbolの位置情報等を保存する
// ツリー構造表示するときに利用

// 以下このソースで有効なstatic関数のプロトタイプ宣言

// ファイルを読み込み、static配列の値を更新する関数
static void count_symbols(const char *filename);

// symbol_count をリセットする関数
static void reset_count(void);

// 与えられた引数でNode構造体を作成し、そのアドレスを返す関数
static Node *create_node(int symbol, int count, Node *left, Node *right);

// Node構造体へのポインタが並んだ配列から、最小カウントを持つ構造体をポップしてくる関数
// n は 配列の実効的な長さを格納する変数を指している（popするたびに更新される）
static Node *pop_min(int *n, Node *nodep[]);

// ハフマン木を構成する関数
static Node *build_tree(void);

// 枝を表示する
static void print_branch(Branch *p);

// 以下 static関数の実装
static void count_symbols(const char *filename)
{
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
	fprintf(stderr, "error: cannot open %s\n", filename);
	exit(1);
    }

    // 1Byteずつ読み込み、カウントする
    /*
      write a code for counting
    */
   unsigned char c;
   while(fread(&c, sizeof(unsigned char), 1, fp) == 1){
       symbol_count[c] += 1;
   }

    fclose(fp);

}
static void reset_count(void)
{
    for (int i = 0 ; i < NSYMBOLS ; i++)
	symbol_count[i] = 0;
}

static Node *create_node(int symbol, int count, Node *left, Node *right)
{
    Node *ret = (Node *)malloc(sizeof(Node));
    *ret = (Node){ .symbol = symbol, .count = count, .left = left, .right = right};
    return ret;
}

static Node *pop_min(int *n, Node *nodep[])
{
    // Find the node with the smallest count
    // カウントが最小のノードを見つけてくる
    int argmin = 0;
    for (int i = 0; i < *n; i++) {
	if (nodep[i]->count < nodep[argmin]->count) {
	    argmin = i;
	}
    }
    
    Node *node_min = nodep[argmin];
    
    // Remove the node pointer from nodep[]
    // 見つかったノード以降の配列を前につめていく
    for (int i = argmin; i < (*n) - 1; i++) {
	nodep[i] = nodep[i + 1];
    }
    // 合計ノード数を一つ減らす
    (*n)--;
    
    return node_min;
}

static Node *build_tree(void)
{
    int n = 0;
    Node *nodep[NSYMBOLS];
    
    for (int i = 0; i < NSYMBOLS; i++) {
	    // カウントの存在しなかったシンボルには何もしない
	    if (symbol_count[i] == 0) continue;
	
	    nodep[n++] = create_node(i, symbol_count[i], NULL, NULL);
    }

    const int dummy = -1; // ダミー用のsymbol を用意しておく
    while (n >= 2) {
	    Node *node1 = pop_min(&n, nodep);
        Node *node2 = pop_min(&n, nodep);

	    // Create a new node
	    // 選ばれた2つのノードを元に統合ノードを新規作成
	    // 作成したノードはnodep にどうすればよいか?

        Node *new = create_node(dummy, node1->count + node2->count , node1, node2);
        // nodep[]の最後 に新しい統合ノードを追加
        nodep[n] = new;
        // ノード数をインクリメント
        n++;
    }

    // なぜ以下のコードで木を返したことになるか少し考えてみよう
    return (n==0)?NULL:nodep[0];
}

// Perform depth-first traversal of the tree
// 深さ優先で木を走査する
// 現状は何もしていない（再帰してたどっているだけ）
void traverse_tree(const int depth, const Node *np)
{			  
    static unsigned char route[MAX_DEPTH] = {-1};

    if (np == NULL)    return;

    if (np->left == NULL) {

        if(np->symbol != 0xa)
            printf("[%c]:  ", np->symbol);
        else
            printf("[%s]: ", "LF");

        for(int i = 0; i < depth; i++){
            printf("%d ",route[i]);
        }

        printf("\n");

        return;
    }

    route[depth] = 0;
    traverse_tree(depth + 1, np->left);
    route[depth] = 1;
    traverse_tree(depth + 1, np->right);

    return;
}

// この関数は外部 (main) で使用される (staticがついていない)
Node *encode(const char *filename)
{
    reset_count();
    count_symbols(filename);

    Node *root = build_tree();
    
    if (root == NULL){
	fprintf(stderr,"A tree has not been constructed.\n");
    }

    return root;
}


 
// 枝 p を含む同じライン上の枝全体を印刷
static void print_branch(Branch *p)
{
    if (p == NULL) {
        return;
    }

    print_branch(p->prev);
    printf("%s",p->s);
}
 
// Treeをツリー状に表示する(　右　→　親　→　左の順で走査)
// position = 左 ? 0: 1 
void print_tree(Node* node, Branch *branch, int position)
{
    if (node == NULL) {
        return;
    }
 
    // 空の枝 next_branch を生成、branchとつなぐ（branch <-- next_branch） 
    Branch *next_branch = (Branch *)malloc(sizeof(Branch));
    next_branch->prev = branch;
    strcpy(next_branch->s, "      ");
 
    // 右探索
    print_tree(node->right, next_branch, 1);
 
    // 葉の直前の枝を設定
    strcpy(next_branch->s, "+-----");


    char tmp_string[] = "      ";

    // 根の枝を設定
    if (!branch) {
        strcpy(next_branch->s, "-----" );
    }
    else {   

        if (position == 1) {
            strcpy(tmp_string, "     |");
        }
        else{
            strcpy(branch->s, "      ");
        }
    }

    // 葉の前の枝を印刷（再帰呼び出し）
    print_branch(next_branch);

    // 葉の情報を印刷
    if(node->left == NULL && node->right == NULL){
        if(node->symbol != 0xa)
            printf(" [%c]\n", node->symbol);
        else
            printf(" [%s]\n", "LF");
    }
    else
        // 途中の疑似ノードを印刷しない
        printf("+  \n");


    if (branch) {
        strcpy(branch->s, tmp_string);
    }

    strcpy(next_branch->s, "     |");
 
    print_tree(node->left, next_branch, 0);

    free(next_branch);
}