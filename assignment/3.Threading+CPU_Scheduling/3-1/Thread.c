///////////////////////////////////////////////////////////////////////
// File Name       : Thread.c                                        //
// Date            : 2020/10/23 ~ 2020/11/05                         //
// OS              : Ubuntu 16.04.3 LTS                              //
// Student Name    : Seung Hoon Jeong                                //
// Student ID      : 2015707003                                      //
// -----------------------------------------                         //
// Title : OS_Assignment3-1                                          //
// Description : integer product within text file using multi-thread //
///////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> /*for clock_gettime() */
#include <sys/mman.h>
#include <pthread.h>

#define MAX_PROCESSES 128
#define MAX_BUFF 1024

long long sum_list[MAX_PROCESSES] = {0,}; // save product result as global variables
long long x=0, y=0; // x+y
int step = 0;
int g_count=0, g_list;
///////////////////////////////////////////////////////////////////////
// Function : void *thread_func(void *arg)                           //
// ================================================================= //
// Input: x                                                          //
// Output: x                                                         //
// Purpose: Function to multiply two integer on text file for thread //
///////////////////////////////////////////////////////////////////////
void *thread_func(void *arg)
{
	int list = (int)arg;
	int count = 0;	
	long long buff[MAX_BUFF];
	char temp[MAX_BUFF];
	bzero(buff,sizeof(buff));
	bzero(temp,sizeof(temp));
    
#ifdef MUTEX
	pthread_mutex_lock(&mutex); // mutex lock
#endif
	// file open //
	FILE *fp1 = fopen("./temp.txt", "rt");
    	if(fp1 == NULL) {
		fprintf(stderr,"file open failed\n");
        	exit(1);
	}
	temp[strlen(temp)-1] = '\0';
	while(fgets(temp,sizeof(MAX_BUFF),fp1) != NULL) {
		buff[count] = atof(temp);
		count++;
		if(count>=MAX_PROCESSES) break;
	}
	
	if(g_count<count) {
		x = buff[g_count];
		y = buff[g_count+1];		
		sum_list[list] = x+y; // perform calculation
		//printf("step %d : %lld + %lld = %lld\n",++step,x,y,sum_list[list]); // check
		g_count+=2;
	}	
	fclose(fp1);
	FILE *fp2 = fopen("./temp.txt", "a"); // open file for appending number
    	if(fp2 == NULL) {
        	fprintf(stderr,"file open failed\n");
        	exit(1);
    	}
    	//fprintf(fp2,"%lld\n",sum_list[list]); // append multiplication result on text file
    	fclose(fp2); // close file
    	g_list = list;
#ifdef MUTEX
	pthread_mutex_unlock(&mutex); // mutex unlock
#endif
	return (void *)1;
}

///////////////////////////////////////////////////////////////////////
// Function : void *thread_result(void *arg)                         //
// ================================================================= //
// Input: x                                                          //
// Output: x                                                         //
// Purpose: multiply the multiplication results again                //
///////////////////////////////////////////////////////////////////////
void *thread_result(void *arg)
{
	int list_ = (int)arg;	
	int first = list_*2;

#ifdef MUTEX
	pthread_mutex_lock(&mutex); // mutex lock
#endif
	FILE *fp3 = fopen("./temp.txt", "a"); // open file for appending number
    	if(fp3 == NULL) {
		fprintf(stderr,"file open failed\n");
    		exit(1);
	}
	
	//printf("step %d : %lld + %lld = ",++step,sum_list[first],sum_list[first+1]);
	sum_list[list_] = sum_list[first] + sum_list[first+1]; // perform result multiplication again
	//printf("%lld\n",sum_list[list_]);	

	fprintf(fp3,"%lld\n",sum_list[list_]); // append multiplication result on text file	
	fclose(fp3); // close file
	
#ifdef MUTEX
	thread_mutex_unlock(&mutex); // mutex unlock
#endif
	return (void *)1;
}

int main(int argc, const char *argv[])
{
	pthread_t p[MAX_PROCESSES];
	pthread_t p_result;
	struct timespec begin, end;
	int lock;
	int i=0;
	clock_gettime(CLOCK_REALTIME, &begin); // start performing time
	
#ifdef MUTEX
	/* declare mutex */
	if((lock = pthread_mutex_init(&mutex,NULL) != 0) { // if mutex error, terminate program
		printf("Mutex initialization error\n");
		return 0;
	}
#endif
		
	for(i=0; i<MAX_PROCESSES; i++){		
		/* Create MAX_PROCESSES threads to perform thread_func */
		pthread_create(&p[i],NULL,thread_func,(void *)i);
		pthread_join(p[i], NULL);
	}
	
	
	int count = MAX_PROCESSES/4; //Factors for calculating in pyramid form
	while(count) {
		for(i=0; i<count; i++) {
			pthread_create(&p_result, NULL, thread_result, (void *)i);
			pthread_join(p_result,NULL);
		}
		count /= 2; // perform contiguously until count==0
	}
	clock_gettime(CLOCK_REALTIME, &end); // end performing time

	printf("Run time : %f\n",((end.tv_sec - begin.tv_sec) + (double)(end.tv_nsec - begin.tv_nsec)/1000000000)); // print runtime

	#ifdef MUTEX
		pthread_mutex_destroy(&mutex); // remove mutex
	#endif

	return 0;
}
