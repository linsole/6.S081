#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    // 创建管道
    int fd[2];
    pipe(fd);
    // 创建一个字符数组用来储存进程接收到的字符
    char buf[12];

    int pid = fork();
    if (pid > 0) {
        write(fd[1], "ping", 4*sizeof(char)); // 把“ping”写到管道里
        wait((int*) 0);
        read(fd[0], buf, 4*sizeof(char)); // 从管道里读4个字符
        printf("%d: received %s\n", getpid(), buf); // 打印出读到的结果
        exit(0);
    } else {
        read(fd[0], buf, 4*sizeof(char)); // 从管道里读4个字符
        printf("%d: received %s\n", getpid(), buf); // 打印出读到的结果
        write(fd[1], "pong", 4*sizeof(char)); // 把“pong”写到管道里
        exit(0);
    }
}
