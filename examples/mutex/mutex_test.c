#include <stdio.h>
#include <pthread.h>

void *thread_inc(void *arg);
void *thread_dec(void *arg);

#ifdef MUTEX
    pthread_mutex_t mutex;
#endif
int a;
int num = 0;

int main()
{
    int status;
    pthread_t t1,t2;
    void *thread_res1, *thread_res2;
#ifdef MUTEX
    if((status = pthread_mutex_init(&mutex, NULL)))
    {
        perror("mutex init");
        return 1;
    }
#endif
    pthread_create(&t1,NULL,thread_inc,"thread1");
    pthread_create(&t2,NULL,thread_dec,"thread2");
    pthread_join(t1,&thread_res1);
    pthread_join(t2,&thread_res2);

    printf("[main] %d\n", num);
#ifdef MUTEX
    pthread_mutex_destroy(&mutex);
#endif
    return 0;
}

void *thread_inc(void *arg)
{
    int i,j=10000;
    for(i=0; i<10; ++i)
    {
#ifdef MUTEX
        pthread_mutex_lock(&mutex);
#endif
        printf("[%s] %d\n", (char*)arg, num);
        a = i;
        while(--j);
        j = 10000;
        num += a;
        printf("[%s] %d\n", (char*)arg, num);
#ifdef MUTEX
    pthread_mutex_unlock(&mutex);
#endif        
    }
    return (void *)num;
}

void *thread_dec(void *arg)
{
    int i,j=10000;
    for(i=0; i<5; ++i)
    {
#ifdef MUTEX
        pthread_mutex_lock(&mutex);
#endif
        printf("[%s] %d\n", (char*)arg, num);
        a = i + 1;
        while(--j);
        j = 10000;
        num -= a;
        printf("[%s] %d\n", (char*)arg, num);
#ifdef MUTEX
    pthread_mutex_unlock(&mutex);
#endif        
    }
    return (void *)num;
}