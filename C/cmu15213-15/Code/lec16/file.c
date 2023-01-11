#include "csapp.h"

int main(void)
{
    char c;
    while(Read(STDIN_FILENO, &c, 1) != 0) // STDIN_FILENO = 0
        Write(STDOUT_FILENO, &c, 1); // STDOUT_FILENO = 1
    exit(0);
} 