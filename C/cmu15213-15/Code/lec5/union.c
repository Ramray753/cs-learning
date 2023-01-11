#include <stdio.h>
#include <string.h>
 
union Data
{
   int i;
   int unsigned j;
   float k;
};
 
int main( )
{
   union Data data;  
   data.i = -1;      
 

   printf( "data.i: %d\n", data.i);
   printf( "data.j: %u\n", data.j);
   printf( "data.k: %f\n", data.k);
 
   return 0;
}