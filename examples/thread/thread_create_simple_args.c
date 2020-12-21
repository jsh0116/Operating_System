#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <assert.h>

void *mythread(void *arg)
{
        long long int value = (long long int)arg;
        printf("%lld\n",value);
        return (void *) (value + 1);
}

int main(int argc, char *argv[])
{
        pthread_t p;
        long long int rvalue;
        pthread_create(&p,NULL,mythread, (void *)100);
        pthread_join(p,(void **)&rvalue);
        printf("returned %lld\n", rvalue);
        return 0;
}

