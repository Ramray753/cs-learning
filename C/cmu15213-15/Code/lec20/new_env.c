#include <stdlib.h>
#include <stdio.h>

extern char **environ;

int main(int argc, char **argv, char **envp){
    printf("\nGet env parameter by environ - execve:\n");
    for (int i = 0; environ[i];){
        printf("%i: %s\n", ++i, environ[i])

    printf("\nGet env parameter by envp - execve:\n");
    for (int i = 0; envp[i];){
        printf("%i: %s\n", ++i, envp[i]);
    }
}