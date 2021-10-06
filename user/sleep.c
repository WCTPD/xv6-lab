#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[]) 
{
  if (argc < 2) {
    printf("Usage: sleep NUMBER...\n");
    exit(0);    
  }
  sleep(atoi(argv[1]) * 10);
  exit(0);
}