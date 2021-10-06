#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

#define DEFAULT_CMD "/bin/echo"

int
main(int argc, char* argv[])
{
  char* exec_cmd[MAXARG] = { 0 };
  char buf[1024] = { 0 };
  
  if (argc == 1) {
    exec_cmd[0] = malloc(sizeof(DEFAULT_CMD) + 1);
    exec_cmd[0][strlen(exec_cmd[0])] = '\0';
  } else {
    if (argc > MAXARG) {
      printf("Too many args\n");
      exit(0);
    }
    for (int i = 1; i < argc; i++) {
      int len = strlen(argv[i]);
      exec_cmd[i - 1] = malloc(len + 1);
      strcpy(exec_cmd[i - 1], argv[i]);
    }
  }

  int args = argc - 1;
  while (1) {
    gets(buf, 1024);
    int len = strlen(buf);
    buf[len - 1] = '\0';
    if (len == 0) {
      break;
    }
    char* p = buf;
    for (int i = 0; i < len; i++) {
      if (buf[i] == ' ') {
        buf[i] = '\0';
        exec_cmd[args] = malloc(buf + i - p + 1);
        strcpy(exec_cmd[args], p);
        args++;
        p = buf + i + 1;
      }
    }
    exec_cmd[args] = malloc(strlen(p) + 1);
    strcpy(exec_cmd[args], p);

    if (fork() == 0) {
      exec(exec_cmd[0], exec_cmd);
    }
    wait(0);
  }


  for (int i = 0; ; i++) {
    if (exec_cmd[i]) {
      free(exec_cmd[i]);
    } else {
      break;
    }
  }

  exit(0);
}