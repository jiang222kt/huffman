#include <stdio.h>
#include <stdlib.h>
#include "encode.h"

int main(int argc, char **argv)
{
    if (argc != 2) {
	fprintf(stderr, "usage: %s <filename>\n",argv[0]);
	exit(1);
    }

    Node *root = encode(argv[1]);

    printf("\n === Code === \n");
    traverse_tree(0,root);

    printf("\n === Tree === \n");
    print_tree(root,NULL,0);

    
    return EXIT_SUCCESS;
}
