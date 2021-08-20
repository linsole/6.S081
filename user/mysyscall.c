#include "kernel/types.h"
#include "kernel/riscv.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  if (mysyscall("hello, world\n") < 0)
    return -1;
  exit(0);
}
