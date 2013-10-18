#ifdef __DEBUG__
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#endif

void mkdir_p(char *path) {
  char *buf;
  char *command = "mkdir -p ";
  int size;

  size = strlen(command) + strlen(path);

  buf = (char *)malloc(size + 1);
  strcpy(buf, command);
  strcat(buf, path);
  buf[size] = '\0';

  system(buf);

  free(buf);
}

#ifdef __DEBUG__
int main() {
  mkdir_p(dirname("hoge/fuga/test.c"));
}
#endif
