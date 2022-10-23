#include <stdio.h>
#include <stdlib.h>

void print_bits(int bits, int len){

    for(int i = len - 1; i >= 0; i--){
        putchar(( 1 << i ) & bits ? '1': '0'); 
    }
    return;

}

void decode_zip(char *filename)
{
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
	fprintf(stderr, "error: cannot open %s\n", filename);
	exit(1);
    }

    int header_size;
    int symbol, bits, bits_len;

    fread(&header_size, sizeof(int), 1, fp);
    printf("header_len is %d\n", header_size);

    for(int i = 0; i < (header_size - 4) / 12; i++){
        fread(&symbol, sizeof(int), 1, fp);
        fread(&bits_len, sizeof(int), 1, fp);
        fread(&bits, sizeof(int), 1, fp); 
        printf("[%c]: ", symbol);
        print_bits(bits, bits_len);
        printf("\n");
    }

    fclose(fp);

}

int main(int argc, char **argv)
{
    if (argc != 2) {
	fprintf(stderr, "usage: %s <filename>\n",argv[0]);
	exit(1);
    }

    decode_zip(argv[1]);

    return EXIT_SUCCESS;
}
