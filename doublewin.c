#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

#define size 48000000
#define combinations 17576000
#define ofs_0 1757600
#define ofs_1 175760
#define ofs_2 17576
#define ofs_3 676
#define ofs_4 26

#define THREADS 8

struct arg_struct {
    unsigned int start;
};


LPCSTR contents;
unsigned char status[combinations] = {0};
unsigned int intervall;
unsigned char doub = 0;

void *check(void *arguments){
    unsigned int index;
    struct arg_struct *args = (struct arg_struct*) arguments;
    unsigned char *ptr = contents + args->start;
    unsigned char *end = ptr + intervall;
    for (; ptr < end; ptr += 8){
        index = (ptr[0]-0x41) + (ptr[1]-0x41)*ofs_4 + (ptr[2]-0x41)*ofs_3 + (ptr[3]-0x30)*ofs_2 + (ptr[4]-0x30)*ofs_1 + (ptr[5]-0x30)*ofs_0;
        if (status[index] != 0){
            doub = 1;
            return NULL;
        }
        status[index] = 1;
    }
    return NULL;
}

int main(int argc, char** argv){
    int i;
    HANDLE tid[THREADS];
    struct arg_struct *arg;
    char * pszFilename = argv[1];

    HANDLE hFileMap;
    HANDLE hFile = CreateFileA(pszFilename, GENERIC_READ, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    hFileMap = CreateFileMappingA(hFile, NULL, PAGE_READONLY, 0, 0, NULL);

    contents = MapViewOfFile(hFileMap, FILE_MAP_READ, 0, 0, size);

    intervall = (size)/THREADS;

    for (i = 0; i < THREADS; i++){
        arg = (struct arg_struct*) malloc(sizeof(struct arg_struct));
        arg->start = i * intervall;
        tid[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) check, arg, 0, NULL);
    }

    for (i = 0; i < THREADS; i++){
        WaitForSingleObject(tid[i], INFINITE);
        if (doub == 1){
            puts("Dubblett");
            return 0;
        }
    }

    puts("Ej dubblett");

    return 0;
}
