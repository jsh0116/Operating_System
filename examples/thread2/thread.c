#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <linux/unistd.h>
#include <stdlib.h>
void *thread_func(void *arg);
void cleanup_func(void *arg);
pid_t gettid(void);

int main(void)
{
	pthread_t tid[2];
	
	pthread_create(&tid[0], NULL, thread_func, (void*)0);
	pthread_create(&tid[1], NULL, thread_func, (void*)1);
	printf("main getpid = %ld\n", (unsigned long)gettid());
	printf("main getpid = %ld\n", (unsigned long)getpid());
	pthread_join(tid[0], NULL);
	pthread_join(tid[1], NULL);
	return 0;
}

void *thread_func(void *arg)
{
	int i;
	//int count = *((int*)arg);
	//char *mydata = (char *)malloc(1000);
	// register fixed final func on stack 
	pthread_cleanup_push(cleanup_func, "first cleanup");
	pthread_cleanup_push(cleanup_func, "second cleanup");
	printf("$tid[%d] start\n",(int)arg);
	printf("$tid[%d] gettid = %ld\n",(int)arg, (unsigned long)gettid());
	printf("$pid[%d] getpid = %ld\n",(int)arg, (unsigned long)getpid());
	for(i=0; i<0x40000000; ++i);
	if((int)arg == 0) pthread_exit(0);
	pthread_cleanup_pop(0);
	pthread_cleanup_pop(1);
	return (void*)1;
}

void cleanup_func(void *arg)
{
	printf("%s\n", (char*)arg);
	return;
}

pid_t gettid(void)
{
	return syscall(__NR_gettid);
}
