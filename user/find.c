#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char*
get_name(char* path)
{
  static char buf[512];
  char *p;
  for (p = path + strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;
  memmove(buf, p, strlen(p));
  buf[strlen(p)] = '\0';
  return buf;
}

void
find(char* path, char* name)
{
  char buf[512] = { 0 };
  int fd, fd1;
  struct dirent de;
  struct stat st, st1;

  // open path
  if ((fd = open(path, 0)) < 0) {
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if (fstat(fd, &st) < 0) {
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  // read items
  while (read(fd, &de, sizeof(de)) == sizeof(de)) {
    if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0 || de.inum == 0) {
      continue;
    }
    int pathlen = strlen(path);
    memmove(buf, path, pathlen);
    *(buf + pathlen) = '/';
    memmove(buf + pathlen + 1, de.name, DIRSIZ);

    if ((fd1 = open(buf, 0)) < 0) {
      fprintf(2, "find: cannot open %s\n", buf);
      continue;
    }

    if (fstat(fd1, &st1) < 0) {
      fprintf(2, "find: cannot stat %s\n", buf);
      close(fd1);
      continue;
    } 

    switch (st1.type) {
    case T_FILE:
      if (strcmp(get_name(buf), name) == 0) {
        printf("%s\n", buf);
      }
      break;

    case T_DIR:
      find(buf, name);
      break;
    }
    close(fd1);
  }
  close(fd);
  return;
}

int
main(int argc, char* argv[])
{
  if (argc < 3) {
    printf("usage: find PATH NAME\n");
    exit(0);
  }
  find(argv[1], argv[2]);
  exit(0);
}
