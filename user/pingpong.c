#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    // create a pipe
    int p[2];
    pipe(p);
    // create a buffer
    char buf[4];
    // fork
    int pid = fork();
    if (pid > 0) {
        int callingPid = getpid();
        write(p[1], "L", 1); // pass a character to the pipe
        pid = wait((int*) 0); // wait the child to finish
        read(p[0], buf, 1); // read a character from the pipe
        printf("%d: received pong\n", callingPid);
        exit(0);
    } else {
        int callingPid = getpid();
        read(p[0], buf, 1);
        printf("%d: received ping\n", callingPid);
        write(p[1], buf, 1);
        exit(0);
    }
}
