#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <unistd.h>

using namespace std;

int t = 0;
pthread_mutex_t mtx[5];
sem_t r;

inline int rand(int l, int r) { return rand() % (r - l + 1) + l; }

inline int rand_time() { return rand(100000, 1000000); }

inline char *get_time() {
  static char str[100];
  time_t t = time(NULL);
  strftime(str, 100, "%Y年%m月%d日 %H:%M:%S", localtime(&t));
  return str;
}

inline void log(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  fprintf(stderr, "[%s]:", get_time());
  vfprintf(stderr, fmt, args);
  fputc('\n', stderr);
}

void *philosopher(void *arg) {
  int id = ((int *)arg)[0];
  while (1) {
    usleep(rand(100000, 500000));
    log("%d饿了，想要就餐", id);
    sem_wait(&r);
    usleep(rand(100000, 500000));
    log("%d等待取左边的%d号筷子", id, id);
    pthread_mutex_lock(&mtx[id]);
    usleep(rand(100000, 500000));
    log("%d已取得左边的%d号筷子，等待取右边的%d号筷子", id, id, (id + 4) % 5);
    pthread_mutex_lock(&mtx[(id + 4) % 5]);
    usleep(rand(100000, 500000));
    log("%d正在就餐...", id);
    pthread_mutex_unlock(&mtx[(id + 4) % 5]);
    pthread_mutex_unlock(&mtx[id]);
    usleep(rand(100000, 500000));
    log("%d吃饱了!", id);
    sem_post(&r);
  }
}

int main() {
  pthread_t p0, p1, p2, p3, p4;
  int id[] = {0, 1, 2, 3, 4};
  sem_init(&r, 0, 4);
  for (int i = 0; i < 5; ++i)
    pthread_mutex_init(&mtx[i], NULL);
  pthread_create(&p0, NULL, philosopher, id);
  pthread_create(&p1, NULL, philosopher, id + 1);
  pthread_create(&p2, NULL, philosopher, id + 2);
  pthread_create(&p3, NULL, philosopher, id + 3);
  pthread_create(&p4, NULL, philosopher, id + 4);
  pthread_join(p0, NULL);
  pthread_join(p1, NULL);
  pthread_join(p2, NULL);
  pthread_join(p3, NULL);
  pthread_join(p4, NULL);
  sem_destroy(&r);
  return 0;
}