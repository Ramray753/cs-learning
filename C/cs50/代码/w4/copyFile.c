#include <stdio.h>
#include <stdlib.h>
#define BUF_SIZE 1024

int copyFile(char* file1, char *file2){
   // 打开文件
   FILE *fp_src = fopen(file1, "r");
   FILE *fp_dest = fopen(file2, "w");
   if(fp_src == NULL || fp_dest == NULL) {
      return(-1);
   }

   // 调整光标
   fseek(fp_src, 0, SEEK_SET);
   
   // 设置缓冲区
   char buffer[BUF_SIZE];

   // 复制文件
   while (fgets (buffer, BUF_SIZE, fp_src) != NULL){
      fputs(buffer, fp_dest);
   }

   // 关闭文件
   fflush(fp_src);
   fclose(fp_src);
   fflush(fp_dest);
   fclose(fp_dest);
   return 0;
}
 
int main(int argc, char **argv){
   return copyFile(argv[1], argv[2]);
}