#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define size 48000000
#define combinations 17576000
#define ofs_0 1757600
#define ofs_1 175760
#define ofs_2 17576
#define ofs_3 676
#define ofs_4 26

int main(int argc, char** argv){
    unsigned int filelength;
    unsigned int i;
    long long reg;
    long long sub = 0x303030414141;
    unsigned char *contents = (unsigned char*) malloc(size);
    /*unsigned char ******occ;*/
    unsigned char *tjenna  = (unsigned char*) malloc(combinations);
    unsigned int index;

    FILE *file = fopen(argv[1], "r");
    fseek(file, 0, SEEK_END);
    filelength = ftell(file);
    rewind(file);
    
    fread(contents, filelength, 1, file);
    memset(tjenna, 0, combinations);

    for (i = 0; i < filelength; i += 8){
        /*reg = (contents[i] & 0xffffffffffff) - sub;*/
        /*printf("%#llx\n", reg);*/

        index = (contents[i]-0x41) + (contents[i+1]-0x41)*ofs_4 + (contents[i+2]-0x41)*ofs_3 + (contents[i+3]-0x30)*ofs_2 + (contents[i+4]-0x30)*ofs_1 + (contents[i+5]-0x30)*ofs_0;
        if (tjenna[index] != 0){
            puts("Dubbletter");
            fclose(file);
            return 0;
        }
        tjenna[index] = 1;
    }
    puts("Ej dubblett");

    fclose(file);

    return 0;
}
