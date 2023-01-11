#include <stdio.h>

int main( )
{
   int unsigned i = 4294967295;
   int j = (int) i;
   printf("%i\n", i);
   printf("%u\n", i);
   printf("%i\n", j);
   printf("%u\n", j);
}