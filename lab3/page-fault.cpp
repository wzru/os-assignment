#include <cstdio>
#include <cstring>
#include <iostream>
#include <unistd.h>

using namespace std;

auto pid = getpid();
int a[2048][2048], b[10240][20480];
int min_flt1, min_flt2, maj_flt1, maj_flt2;

void exec_cmd(const char *cmd, char *res) {
  char buf_ps[1024];
  char ps[1024] = {0};
  FILE *ptr;
  strcpy(ps, cmd);
  if ((ptr = popen(ps, "r")) != NULL) {
    while (fgets(buf_ps, 1024, ptr) != NULL) {
      strcat(res, buf_ps);
      if (strlen(res) > 1024)
        break;
    }
    pclose(ptr);
    ptr = NULL;
  } else {
    printf("popen %s error\n", ps);
  }
}

inline void get_pgfault(int &min_flt, int &maj_flt) {
  static char cmd[1024], res[1024];
  int tmp;
  res[0] = '\0';
  sprintf(cmd, "ps -o pid,min_flt,maj_flt %d | grep %d", pid, pid);
  exec_cmd(cmd, res);
  sscanf(res, "%d%d%d", &tmp, &min_flt, &maj_flt);
  // printf("%s", res);
  // printf("min=%d, maj=%d\n", min_flt, maj_flt);
}

int main() {
  printf("---a[2048][2048]---\n");
  get_pgfault(min_flt1, maj_flt1);
  for (int i = 0; i < 2048; ++i)
    for (int j = 0; j < 2048; ++j)
      a[i][j] = 1;
  get_pgfault(min_flt2, maj_flt2);
  printf("min_flt=%d, maj_flt=%d\n", min_flt2 - min_flt1, maj_flt2 - maj_flt1);
  printf("---b[10240][20480]---\n");
  get_pgfault(min_flt1, maj_flt1);
  for (int i = 0; i < 10240; ++i)
    for (int j = 0; j < 20480; ++j)
      b[i][j] = 2;
  get_pgfault(min_flt2, maj_flt2);
  printf("min_flt=%d, maj_flt=%d\n", min_flt2 - min_flt1, maj_flt2 - maj_flt1);
  return 0;
}