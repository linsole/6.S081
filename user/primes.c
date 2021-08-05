#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    // 创建数组初始化，储存每一轮中待处理的数字
    int numbers[34];
    int index = 0; // index用来记录每一轮中待处理的数字的个数
    for (int i = 2; i <= 35; i += 1) {
        numbers[index++] = i;
    }
    // 用来存放file descriptor的数组
    int fd[2];

    // 循环直到待处理的数字个数为0为止
    while (index > 0) {
        pipe(fd); // 每一轮循环建立一个新的管道
        int pid = fork();
        // 父进程
        if (pid > 0) {
            close(fd[0]);
            for (int i = 0; i < index; i += 1) {
                write(fd[1], numbers+i, sizeof(int));
            }
            close(fd[1]);
            wait((int *) 0);
            exit(0);  //在这里退出父进程
        } else { // 子进程
            int temp;
            int devider;
            index = 0; // index置0，为了在该进程中计算出处理后的数字的个数
            close(fd[1]); //必须在这里先关闭管道的写入端，否则会造成子进程陷入死循环

            // 待处理的数字中第一个就是素数，也是接下来运算过程中的除数
            read(fd[0], &devider, sizeof(int));
            printf("prime %d\n", devider);
            while (read(fd[0], &temp, sizeof(int)) != 0) {
                if (temp % devider != 0) {
                    numbers[index++] = temp;
                }
            }
            close(fd[0]); 
            //最后不用exit退出子进程，到下一个循环开始，该进程就成为父进程了
        }
    }
    exit(0); // 用return 0 会产生错误，目前还不清楚原因
}
