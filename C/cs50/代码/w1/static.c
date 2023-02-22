#include <stdio.h>
 
void func1(void);
 
static int count=10;   
 
int main()
{
  while (count--) {
      func1();
  }
}
 
void func1(void)
{             
  static int thingy = 5;
  thingy++;
  printf(" thingy 为 %d ， count 为 %d\n", thingy, count);
}