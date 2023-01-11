#include <stdio.h>
#include <unistd.h>
#include <string.h>

extern char **environ;

int main(int argc, char **argv, char **envp){
    int i = 0;
    for(i = 0; i < argc; i++){
        printf("argv %i: %s\n", i, argv[i]);
    }
    i = 0;
    while(environ[i]) {
        printf("envp: %s\n", environ[i++]); // prints in form of "variable=value"
    }
    i = 0;
    while(envp[i]) {
        printf("envp: %s\n", envp[i++]); // prints in form of "variable=value"
    }
}

