#include <cs50.h>
#include <stdio.h>
int main(int argc, char* *argv){
    for(int i = 0; i < argc; i++){
        printf("arg[%i]: %s\n", i, argv[i]);
    }
}