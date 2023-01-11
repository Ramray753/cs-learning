#include <stdio.h>
#include <unistd.h>

int main(void){
    int i = 0;
    while(1){
        sleep(10);
        // printf("Sleep %i\n", ++i);
    }
}