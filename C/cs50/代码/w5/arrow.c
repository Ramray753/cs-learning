#include <stdio.h>

typedef struct{
   int val;
   void *next;
} Node;

int main(void){
    Node n2 = {100, NULL};
    Node n1 = {200, &n1};
    printf("%d\n", n1.val);
    printf("%d\n", ((Node*) n1.next) -> val);
}