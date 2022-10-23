#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "encode.h"
#include "myzip.h"

// 以下このソースで有効なstatic関数のプロトタイプ宣言

// symbolに対応するcode を code listから検索
static Code * find_code(unsigned char c, Code *h);

// code lsit のメモリを解
static void free_codelist(Code *head);

// input ファイルを code に基づき圧縮し、outputに出力
void zip_to_file(char *input, char *output);

extern Code *head;

static Code * find_code(unsigned char c, Code *h){
    Code *tmp = h;
    while(tmp != NULL){
        if (tmp->symbol == c){
            return tmp;
        }
        tmp = tmp->next;
    }

    return NULL;
}

// Header の構造：ヘッダ長(int)の後に、各コード（symbol, bits_len, bits)を順に格納
// header_size(int), code1(symbol(int), bits_len(int), bits(int)), 
// code2(symbol(int), bits_len(int), bits(int)), ..
void zip_to_file(char *input, char *output)
{

    FILE *fpin = fopen(input, "rb");
    if (fpin == NULL) {
	fprintf(stderr, "error: cannot open %s\n", input);
	exit(1);
    }

    FILE *fpout = fopen(output, "wb");
    if (fpout == NULL) {
	fprintf(stderr, "error: cannot open %s\n", output);
	exit(1);
    }

    int header_size = 0;
    int num_of_code = 0;

    Code *tmp = head;
    while(tmp != NULL){
        num_of_code += 1;
        tmp = tmp->next;
    }

    header_size = sizeof(int) * 3 * num_of_code + sizeof(int);

    fwrite(&header_size, sizeof(int), 1, fpout);

    tmp = head;
    while(tmp != NULL){
        fwrite(&tmp->symbol, sizeof(tmp->symbol), 1, fpout);
        fwrite(&tmp->bits_len, sizeof(tmp->bits_len), 1, fpout);
        fwrite(&tmp->bits, sizeof(tmp->bits), 1, fpout);       
        tmp = tmp->next;
    }

    unsigned char buffer;  // symbolに対応するcodeをbit列に詰め込むためのbuffer
    int byte_position;  // bufferの空き位置 
    unsigned char c;

    buffer = buffer & 0;
    byte_position = 0;
    while(fread(&c, sizeof(unsigned char), 1, fpin) == 1){
        tmp = find_code(c, head);
        if (tmp == NULL){
            fprintf(stderr, "Cant not find code for symbol[%c]\n", tmp->symbol);
            exit(EXIT_FAILURE);
        }
        // little endian のため、メモリ上にある整数(tmp->bits)からbitを取り出す方向を注意
        // 例えば、8 (1000) のメモリ上の並び：　00010000000000000000000 となる
        for(int i = tmp->bits_len - 1; i >= 0; i--){
            if( byte_position <= 7){
                buffer = buffer + ( tmp->bits & ((unsigned char)1 << i));
                byte_position += 1;
            }
            if( byte_position > 7){
                fwrite(&buffer, sizeof(unsigned char), 1, fpout);
                byte_position = 0;
                buffer = buffer & 0;
            }
        }

    } 

    fclose(fpin);
    fclose(fpout);

    // code listのメモリを解放する
    free_codelist(head);

    return;
}

// code listのメモリを解放
static void free_codelist(Code *head)
{
    Code *tmp = head -> next;
    while(head != NULL){
        tmp = head->next;
        free(head);
        head = tmp;
    }

    return;
}

