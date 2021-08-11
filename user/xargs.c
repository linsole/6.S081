#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    if (fork() > 0) {
        wait((int *)0);
    } else {
        // 声明一个字符串指针数组，用来存放传来的程序的参数和标准输入传来的参数
        char *arg[MAXARG];
        // 先把xargs程序本身接收到的参数argv存起来
        int args_index = 0;
        for (int i = 1; i < argc; i++) arg[args_index++] = argv[i];

        char buf[10]; // 临时存放字符串
        char *p = buf;
        while (read(0, p, sizeof(char)) != 0 && args_index < MAXARG) {
            if (*p == ' ' || *p == '\n') {
                *p = '\0'; // 给字符串结尾'\0'
                p = buf; // p重置
                arg[args_index] = (char *)malloc(strlen(p)*sizeof(char)); // 先分配一段内存
                strcpy(arg[args_index++], p); // 把得到的参数复制到字符串指针数组当中
            } else {
                p++;
            }
        }
        exec(argv[1], arg);
    }
    exit(0);
}
