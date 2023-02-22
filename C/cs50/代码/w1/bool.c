#include <stdio.h>
#include <stdbool.h>

int main(void) {
    bool keep_going = true;  //也可以是bool keep_going = 1;
    while(keep_going) {
        printf("本程序会在keep_going为真时持续运行。\n");
        keep_going = false;    // 也可以是 keep_going = 0;`
    }
    printf("停止运行！\n");
}