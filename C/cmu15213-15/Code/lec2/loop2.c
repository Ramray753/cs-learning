#include <stdio.h>
#include <stdlib.h>
#define SIZE 5000

void copy(int src[SIZE][SIZE], int dest[SIZE][SIZE]){
    for (int j = 0; j < SIZE; j++){
        for (int i = 0; i < SIZE; i++){
            dest[i][j] = src[i][j];
        }
    }
}

int main(void){
    int (*a)[SIZE] = malloc(sizeof(int[SIZE][SIZE]));
    int (*b)[SIZE] = malloc(sizeof(int[SIZE][SIZE]));
    copy(a, b);
    free(a);
    free(b);
}