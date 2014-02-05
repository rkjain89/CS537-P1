#include "types.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  int numCalls = getsyscallinfo();
  printf (1, "Total number of system calls %d\n", numCalls);
  exit();
}
