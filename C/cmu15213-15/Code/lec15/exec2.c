#include <stdio.h>

extern char **environ;

int main(int argc, char **argv, char **envp){
    int i = 0;
    while(environ[i]) {
        printf("%i: %s\n", i, environ[i++]); 
    }
    i = 0;
    while(envp[i]) {
        printf("%i: %s\n", i, envp[i++]); 
    }
}