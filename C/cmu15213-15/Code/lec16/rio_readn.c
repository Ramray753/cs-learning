#include "csapp.h"

int main(void)
{
    char c[10];
    rio_readn(STDIN_FILENO, c, 10); // STDIN_FILENO = 0
    rio_writen(STDOUT_FILENO, c, 5); // STDOUT_FILENO = 1
    exit(0);
} 