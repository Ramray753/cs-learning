#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

extern char **environ;

int main(void){
    printf("Get env parameter by environ:\n");
    for (int i = 0; i < 3;){
        printf("%i: %s\n", ++i, environ[i]);
    }

    printf("\nSet env parameter by setenv:\n");
    setenv("NEW_VAR_1", "Ramray_1", 1);
    setenv("NEW_VAR_2", "Ramray_2", 1);
    
    printf("\nUnset env parameter by setenv:\n");
    unsetenv("NEW_VAR_2");

    printf("\nGet env parameter by getenv:\n");
    printf("%s: %s\n", "NEW_VAR_1", getenv("NEW_VAR_1"));
    printf("%s: %s\n", "PWD", getenv("PWD"));
    printf("%s: %s\n", "HOME", getenv("HOME"));

    printf("\nGet non-existing env parameter by getenv:\n"); // 不会报错
    printf("%s: %s\n", "NEW_VAR_2", getenv("NEW_VAR_2"));

    // fork子进程自动继承变量
    int pid;
    if ((pid = fork()) == 0) {
        printf("\nGet env parameter by environ - fork:\n");
        for (int i = 0; i < 3;){
            printf("%i: %s\n", ++i, environ[i]);
        }
        printf("\nGet env parameter by getenv - fork:\n");
        printf("%s: %s\n", "NEW_VAR_1", getenv("NEW_VAR_1"));
    }

    // execve子进程主动传递环境变量
    char *envp[]={"NEW_ENV=123456789", NULL};
    char *args[]={"./new_env", NULL};
    if ((pid = fork()) == 0) {
        execve(args[0], args, envp);
    }
}