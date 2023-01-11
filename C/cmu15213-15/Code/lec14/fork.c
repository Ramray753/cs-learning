#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

int main(void){
    int pid;
    if ((pid = fork()) < 0){
        fprintf(stderr, "fork error: %s\n", strerror(errno));
        exit(0);
    }
    printf("Hello, world!\n");
}