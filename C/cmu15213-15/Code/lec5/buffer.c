#include <stdio.h>

void echo(){
    char s[4];
    gets(s);
    puts(s);
}

void call_echo(void){
    echo();
}

int main(void){
    printf("Input: ");
    call_echo();
}