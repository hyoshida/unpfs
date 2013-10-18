#ifdef __DEBUG__
#include <stdio.h>
#include <string.h>
#endif

void strtr(const char *string, char src, char dst) {
  char *p;
  while ((p = strchr(string, src)) != '\0') {
    *p = dst;
  }
}

#ifdef __DEBUG__
int main() {
  char buf[10];

  strcpy(buf, "hoge\\fuga");
  buf[sizeof(buf) - 1] = '\0';

  strtr(buf, '\\', '/');

  printf("%s\n", buf);
}
#endif
