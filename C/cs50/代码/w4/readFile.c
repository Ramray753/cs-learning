#include <stdio.h>
#include <stdlib.h>
 
int main()
{
   char *buffer = malloc(256);
 
   /* 打开文件用于读写 */
   FILE *fp = fopen("hello.txt", "r");

   /* 获取文件字节数 */
   fseek(fp, 0, SEEK_END);
   int len = ftell(fp);

   /* 查找文件的开头 */
   fseek(fp, 0, SEEK_SET);
 
   /* 读取并显示数据 */
   fread(buffer, len, 1, fp);
   buffer[len] = '\0';
   printf("%s\n", buffer);
   
   /* 关闭文件 */
   fflush(fp);
   fclose(fp);
   free(buffer);
}