
#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char* argv[])
{
  int p1[2];
  int p2[2];

  char* buf = malloc(1024);

  pipe(p1);
  pipe(p2);

  if (fork() == 0) {
    // child
    close(p1[1]);  
    close(p2[0]);
    read(p1[0], buf, 5);
    close(p1[0]);
    printf("%d: received %s\n", getpid(), buf);
    write(p2[1], "pong", 5);
    close(p2[1]);
  } else {
    // parent
    close(p1[0]);
    close(p2[1]);
    write(p1[1], "ping", 5);
    close(p1[1]);
    read(p2[0], buf, 5);
    printf("%d: received %s\n", getpid(), buf);
    close(p2[0]);
  }
  exit(0);
}