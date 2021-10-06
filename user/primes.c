#include "kernel/types.h"
#include "user/user.h"

void prime(int p);

int
main()
{
  int p[2];
  pipe(p);
  if (fork() == 0) {
    // child
    close(p[1]);
    prime(p[0]);
    close(p[0]);
  } else {
    // parent
    close(p[0]);
    for (int i = 2; i <= 35; i++) {
      write(p[1], &i, sizeof(int));
    }    
    close(p[1]);
  }
  wait(0);
  exit(0);
}

void 
prime(int p)
{
  int base;
  if (read(p, &base, sizeof(int)) == 0) {
    exit(0);
  }
  printf("prime %d\n", base);
  int p1[2];
  pipe(p1);

  if (fork()) {
    // child
    close(p1[1]);
    prime(p1[0]);
  } else {
    // parent
    int n;
    close(p1[0]);
    while (read(p, &n, sizeof(int))) {
      if (n % base) {
        write(p1[1], &n, sizeof(int));
      }
    } 
  }
  wait(0);
  exit(0);
}