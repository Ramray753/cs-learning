#include <stdio.h>
#include <unistd.h>

int main(void){
    char *args[]={"./exec_fork", "Hello", "World", NULL};
    char *envp[]={"NEW_ENV=123456789", NULL};
    execve(args[0], args, envp);
    printf("Hello, World! From C...\n");
}
