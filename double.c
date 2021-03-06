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

#define THREADS 4

struct arg_struct {
    unsigned int start;
    unsigned char *doub;
};

/*unsigned char lookup[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25};*/

unsigned char *contents;
unsigned char status[combinations] = {0};
unsigned int intervall;

void *check(void *arguments){
    unsigned int index;
    struct arg_struct *args = (struct arg_struct*) arguments;
    unsigned char *ptr = contents + args->start;
    unsigned char *end = ptr + intervall;
    for (; ptr < end; ptr += 8){
        index = (ptr[0]-0x41) + (ptr[1]-0x41)*ofs_4 + (ptr[2]-0x41)*ofs_3 + (ptr[3]-0x30)*ofs_2 + (ptr[4]-0x30)*ofs_1 + (ptr[5]-0x30)*ofs_0;
        /*index = lookup[ptr[0]] + lookup[ptr[1]]*ofs_4 + lookup[ptr[2]]*ofs_3 + lookup[ptr[3]]*ofs_2 + lookup[ptr[4]]*ofs_1 + lookup[ptr[5]]*ofs_0;*/
        if (status[index] != 0){
            *args->doub = 1;
            return NULL;
        }
        status[index] = 1;
    }
    return NULL;
}

int main(int argc, char** argv){
    int file;
    struct stat s;
    size_t filesize;
    int i;
    pthread_t tid[THREADS * sizeof(pthread_t)];
    struct arg_struct *arg;
    unsigned char doub = 0;

    file = open(argv[1], O_RDONLY);
    fstat(file, &s);
    filesize = s.st_size;
    contents = mmap(0, filesize, PROT_READ, MAP_PRIVATE, file, 0);

    intervall = (filesize)/THREADS;

    for (i = 0; i < THREADS; i++){
        arg = (struct arg_struct*) malloc(sizeof(struct arg_struct));
        arg->start = i * intervall;
        arg->doub = &doub;
        pthread_create(&tid[i], NULL, check, arg);
    }

    for (i = 0; i < THREADS; i++){
        pthread_join(tid[i], NULL);
        if (doub == 1){
            puts("Dubblett");
            return 0;
        }
    }

    puts("Ej dubblett");

    return 0;
}
