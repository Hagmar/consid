#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>

#define size 48000000
#define combinations 17576000
#define ofs_0 1757600
#define ofs_1 175760
#define ofs_2 17576
#define ofs_3 676
#define ofs_4 26

struct arg_struct {
    unsigned int start;
    unsigned int intervall;
    unsigned char *doub;
    pthread_mutex_t *mutex;
};

unsigned char lookup[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25};

unsigned char *contents;
unsigned char *status;

void *check(void *arguments){
    unsigned int index;
    struct arg_struct *args = (struct arg_struct*) arguments;
    unsigned char *ptr = contents + args->start;
    unsigned char *end = ptr + args->intervall;
    /*pthread_mutex_t *mutex = args->mutex;*/
    for (; ptr < end; ptr += 8){
        index = (ptr[0]-0x41) + (ptr[1]-0x41)*ofs_4 + (ptr[2]-0x41)*ofs_3 + (ptr[3]-0x30)*ofs_2 + (ptr[4]-0x30)*ofs_1 + (ptr[5]-0x30)*ofs_0;
        /*index = lookup[ptr[0]] + lookup[ptr[1]]*ofs_4 + lookup[ptr[2]]*ofs_3 + lookup[ptr[3]]*ofs_2 + lookup[ptr[4]]*ofs_1 + lookup[ptr[5]]*ofs_0;*/
        /*pthread_mutex_lock(mutex);*/
        if (status[index] != 0){
            *args->doub = 1;
        }
        status[index] = 1;
        /*pthread_mutex_unlock(mutex);*/
    }
    return NULL;
}

int main(int argc, char** argv){
    int file;
    struct stat s;
    size_t filesize;
    /*unsigned char *ptr;*/
    /*unsigned int index;*/
    int i;

    file = open(argv[1], O_RDONLY);
    fstat(file, &s);
    filesize = s.st_size;
    contents = mmap(0, filesize, PROT_READ, MAP_PRIVATE, file, 0);


    /*unsigned int filelength;*/
    /*long long reg;*/
    /*int j,k,l,m;*/
    /*long long sub = 0x303030414141;*/
    status = (unsigned char*) calloc(combinations, 1);

    /*unsigned char ******occ = (unsigned char******) malloc(combinations*2);*/
    /*memset(occ, 0, combinations);*/
    /*for (i = 0; i < 26; i++){*/
        /*occ[i] = (unsigned char*****) occ+(i+1)*ofs_4;*/
    /*}*/
    /*for (i = 0; i < 26; i++){*/
        /*occ[i] = occ_i;*/
        /*for (j = 0; j < 26; j++){*/
            /*occ_i[j] = occ_j;*/
            /*for (k = 0; k < 26; k++){*/
                /*occ_j[k] = occ_k;*/
                /*for (l = 0; l < 10; l++){*/
                    /*occ_k[l] = occ_l;*/
                    /*for (m = 0; m < 10; m++){*/
                        /*occ_l[m] = occ_m;*/
                        /*memset(occ_m, 0, 10);*/
                    /*}*/
                /*}*/
            /*}*/
        /*}*/
    /*}*/

    /*int = fopen(argv[1], "r");*/
    /*fseek(file, 0, SEEK_END);*/
    /*filelength = ftell(file);*/
    /*rewind(file);*/
    
    /*fread(contents, filelength, 1, file);*/
    pthread_t *tid = malloc( 4 * sizeof(pthread_t) );
    struct arg_struct *arg;
    unsigned int intervall = (filesize)/4;
    unsigned char doub = 0;
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    for (i = 0; i < 4; i++){
        arg = (struct arg_struct*) malloc(sizeof(struct arg_struct));
        arg->start = i * intervall;
        arg->intervall = intervall;
        arg->doub = &doub;
        arg->mutex = &mutex;
        pthread_create(&tid[i], NULL, check, arg);
    }

    for (i = 0; i < 4; i++){
        pthread_join(tid[i], NULL);
    }

    if (doub == 1){
        puts("Dubblett");
    } else {
        puts("Ej dubblett");
    }

    return 0;
}
