#include <cs50.h>
#include <stdio.h>

int main(void) {
    int h = get_int("Height: ");
    for (int i = 1; i <= h; i++){
        for (int j = 0; j < h - i; j++){
            printf(" ");
        }
        for (int j = 0; j < i; j++){
            printf("#");
        }
        printf("  ");
        for (int j = 0; j < i; j++){
            printf("#");
        }
        for (int j = 0; j < h - i; j++){
            printf(" ");
        }
        printf("\n");
    }
}