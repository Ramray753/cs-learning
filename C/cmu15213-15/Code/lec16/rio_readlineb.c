#include "csapp.h"

int main(void)
{
    char c = 'A';
    printf("Before read: c = %c\n", c);
    rio_t rio; 
    Rio_readinitb(&rio, STDIN_FILENO); 
    rio_readlineb(&rio, &c, 1);
    printf("After read: c = %c\n", c);
    exit(0);
} 