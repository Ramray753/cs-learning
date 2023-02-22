#include <stdio.h>
#include <string.h>
#include <stdlib.h>
 
int main()
{
   char buffer[] = "New line 1\n";
 
   /* 打开文件用于读写 */
   FILE *fp = fopen("hello.txt", "a");

   /* 查找文件的末尾 */
   fseek(fp, 0, SEEK_END);
 
   /* 读取并显示数据 */
   fwrite(buffer, strlen(buffer), 1, fp);
   fputs("New Line 2\n", fp);
   fprintf(fp, "New Line %d\n", 3);
   
   /* 关闭文件 */
   fflush(fp);
   fclose(fp);
}