#include <stdio.h>
#include <stdlib.h>
#include "encode.h"
#include "myzip.h"

int main(int argc, char **argv)
{
    if (argc != 3) {
	fprintf(stderr, "usage: %s <intput filename> <output filename>\n",argv[0]);
	exit(1);
    }

    Node *root = encode(argv[1]);

    traverse_tree(0,root);
    
    zip_to_file(argv[1], argv[2]);

    return EXIT_SUCCESS;
}
