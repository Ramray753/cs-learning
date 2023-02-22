#include <string.h>
#include <stdio.h>

int main(void){
   char *s1 = "Apple";
   int l = strlen(s1);
   char s2[l + 1];
   for (int i = 0; i <= l; i++){
      s2[i] = s1[i];
   }
   printf("%s\n", s1);
   printf("%s\n", s2);
   s2[0] = 'a';
   printf("%s\n", s1);
   printf("%s\n", s2);
 }