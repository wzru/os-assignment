#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <unistd.h>

using namespace std;

int t = 0;
pthread_mutex_t mutex;
int buf[10], cnt = 0;

void push(int x) {
  buf[cnt++] = x;
  if (cnt > 10)
    cnt = 10;
}

int pop() {
  if (cnt > 0)
    return buf[--cnt];
  else
    return 0;
}

inline int rand(int l, int r) { return rand() % (r - l + 1) + l; }

inline int rand_time() { return rand(100000, 1000000); }

inline char *get_time() {
  static char str[100];
  time_t t = time(NULL);
  strftime(str, 100, "%Y年%m月%d日 %H:%M:%S", localtime(&t));
  return str;
}

void *producer(void *arg) {
  int id = ((int *)arg)[0], l = ((int *)arg)[1], r = ((int *)arg)[2];
  while (1) {
    pthread_mutex_lock(&mutex);
    int x = rand(l, r);
    push(x);
    printf("[%s]:\e[1;42mP%d produced %d\e[0m\n", get_time(), id, x);
    pthread_mutex_unlock(&mutex);
    usleep(rand_time());
  }
}

void *consumer(void *arg) {
  int id = ((int *)arg)[0];
  while (1) {
    t = time(NULL);
    pthread_mutex_lock(&mutex);
    int x = pop();
    if (x)
      printf("[%s]:\e[1;44mC%d consumed %d\e[0m\n", get_time(), id, x);
    pthread_mutex_unlock(&mutex);
    usleep(rand_time());
  }
}

int main() {
  printf("是否将生产消费记录保存到文件produce-consume.txt?(y/N)");
  char ch;
  scanf("%c", &ch);
  if (ch == 'y' || ch == 'Y') {
    printf("日志将写入文件produce-consume.txt...");
    freopen("produce-consume.txt", "w", stdout);
  }
  pthread_t p1, p2;
  pthread_t c1, c2, c3;
  int b1[] = {1, 1000, 1999}, b2[] = {2, 2000, 2999};
  int id[] = {1, 2, 3};
  pthread_mutex_init(&mutex, NULL);
  pthread_create(&p1, NULL, producer, b1);
  pthread_create(&p2, NULL, producer, b2);
  pthread_create(&c1, NULL, consumer, id);
  pthread_create(&c2, NULL, consumer, id + 1);
  pthread_create(&c3, NULL, consumer, id + 2);
  pthread_join(p1, NULL);
  pthread_join(p2, NULL);
  pthread_join(c1, NULL);
  pthread_join(c2, NULL);
  pthread_join(c3, NULL);
  return 0;
}