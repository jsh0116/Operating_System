#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/sem.h>
#include <pthread.h>
#include <sys/types.h>

//#include <semaphore.h>
#include <time.h>

#define MAX_THREAD 5
#define MAX_RESOUCE 3
#define MAX_BUF 1024
int g_semid = 0;

char *getTime(char *buffer)
{
	time_t now = time(NULL);
	struct tm *t = localtime(&now);
	sprintf(buffer, "%02d:%02d:%02d",t->tm_hour, t->tm_min, t->tm_sec);
	return buffer;
}

void *thread_func(void *arg)
{
	int i;
	unsigned int tid = pthread_self();
	struct sembuf s;
	char buff[MAX_BUF];
	s.sem_num = 0; // semaphore number to control
	s.sem_flg = SEM_UNDO;
	for(i=0; i<MAX_THREAD; ++i)
	{
		/* 
		sem_op = semaphore value 계산하기 위해 설정하는 값 
		if neg : semval이 충분히 크다면 sem_op만큼 감소
			0 : semval이 0이 될때까지 기다린다. if semval !=0 && IPC_NOWAIT return -1, errno = EAGAIN
			pos : semval을 sem_op만큼 증가시킴.
		*/
		// P()
		s.sem_op = -1;
		if(semop(g_semid,&s,1) < 0)
			perror("semop");
		
		printf("%u %s enter.\n", tid, getTime(buff));
		sleep(2);
		printf("%u %s leave.\n", tid, getTime(buff));

		// V()
		s.sem_op = 1;
		if(semop(g_semid,&s,1) < 0)
			perror("semop");
	}
	return NULL;
}

int main()
{
	int i;
	union semun sem_union;
	pthread_t thread[MAX_THREAD];
	
	/* semaphore가 1개인 1234 key의 semaphore 집합 생성 */
	if((g_semid = semget((key_t)0x1234,1,IPC_CREAT | 0666)) < 0) {
		perror("semget");
		exit(1);
	}

	sem_union.val = MAX_RESOUCE;
	/* semaphore 값을 MAX_RESOURCE로 설정. */
	if(semctl(g_semid, 0, SETVAL, sem_union) < 0) {
		perror("semctl");
		exit(1);
	}

	for(i=0; i<MAX_THREAD; ++i)
		pthread_create(&thread[i],NULL,thread_func,NULL);
	for(i=0; i<MAX_THREAD; ++i)
		pthread_join(thread[i],NULL);

	/* remove semaphore */
	if(semctl(g_semid,0,IPC_RMID,0) < 0) {
		perror("semctl");
		exit(1);
	}

	return 0;
}
