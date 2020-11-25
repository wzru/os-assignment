#include <pthread.h>
#include <cstdio>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <wait.h>
#include <iostream>

using namespace std;

typedef void *(*thread_func)(void *);

const int dur = 20000;

void A()
{
    for (int i = 1; i <= 1000; ++i)
    {
        printf("A:%04d\n", i);
        usleep(dur);
    }
}

void B()
{
    for (int i = 1000; i >= 1; --i)
    {
        printf("B:%04d\n", i);
        usleep(dur);
    }
}

int main()
{
    pthread_t ta, tb;
    pthread_create(&ta, NULL, (thread_func)A, NULL);
    pthread_create(&tb, NULL, (thread_func)B, NULL);
    pthread_join(ta, NULL);
    pthread_join(tb, NULL);
    return 0;
}