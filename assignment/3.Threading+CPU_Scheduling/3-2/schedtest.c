//////////////////////////////////////////////////////////////
// File Name       : schedtest.c                            //
// Date            : 2020/10/23 ~ 2020/11/05                //
// OS              : Ubuntu 16.04.3 LTS                     //
// Student Name    : Seung Hoon Jeong                       //
// Student ID      : 2015707003                             //
// -----------------------------------------                //
// Title : OS_Assignment3-2                                 //
// Description : CPU Scheduling performance test            //
//////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sched.h>
#include <linux/sched.h>
//#include <pthread.h>

#define MAX_PROCESSES 10000
#define MAX_BUFF 1024
#define MAX_NICE 19
#define MIN_NICE -20

int main()
{
    struct timespec begin, end;
    struct sched_param param;
    pid_t pid;
    char dir[MAX_BUFF] = {0,};
    char file_num[MAX_BUFF] = {0,};
    int fd = 0, num=0, priority_num=0, nice_num=0;
    char buff[MAX_BUFF] = {0,};
    printf("Here is the CPU Scheduler.\n");
    printf("0: The standard round-robin time-sharing policy.\n");
    printf("1: a first-in first-out policy.\n");
    printf("2: a round-robin policy.\n");
    printf("please choose number: ");
    scanf("%d",&num);
    if(num != 0 && num != 1 && num != 2) {
        printf("Please choose number 0 or 1 or 2. Try again.\n");
        exit(0);
    }
    printf("Here is the scheduling priority.\n");
    printf("0(min), 50(default or mid), 99(max)\n");
    printf("please choose number: ");
    scanf("%d",&priority_num);
    if(priority_num != 0 && priority_num != 50 && priority_num != 99) {
        printf("Please choose number 0 or 50 or 99. Try again.\n");
        exit(0);
    }
    printf("Here is the nice value list.\n");
    printf("-20(min), 0(default or mid), 19(max)\n");
    printf("please choose number: ");
    scanf("%d",&nice_num);
    if(nice_num != -20 && nice_num != 0 && nice_num != 19) {
        printf("Please choose number -20 or 0 or 19. Try again.\n");
        exit(0);
    }
    /*
                     priority range
        SCHED_OTHER:        0
        SCHED_FIFO:       0~99
        SCHED_RR:         0~99
    */
    /* scheduler normal test == 0, FIFO,RR == 1 */ 
    if(priority_num == 0)
        param.sched_priority = sched_get_priority_min(num);
    else if(priority_num == 99)
        param.sched_priority = sched_get_priority_max(num);
    else 
        param.sched_priority = (sched_get_priority_min(num) + sched_get_priority_max(num))/2;
    
    if(nice_num==19)
        nice(MAX_NICE);
    else if(nice_num==-20)
        nice(MIN_NICE);
    else
        nice(0);

    clock_gettime(CLOCK_REALTIME,&begin); // start measuring performance time
    
    for(int i=0; i<MAX_PROCESSES; i++) // create PROCESS(num:MAX_PROCESSES)
    {
        getcwd(dir,MAX_BUFF);
        strcat(dir,"/tmp1/");
        sprintf(file_num,"%d",i);
        strcat(dir,file_num);

        if((pid =fork()) < 0) {
            perror("fork");
            return -1;
        }
        else if(pid == 0) { // child process
            /* Change CPU Scheduling Algorithm(FIFO, RR, OTHER) */
            if(sched_setscheduler(getpid(), num, &param) != 0) {
                perror("Changing CPU Scheduler");
                return 0;
            }
            
            if((fd = open(dir,O_RDONLY)) > 0) {
                read(fd,buff,MAX_BUFF);
                close(fd);
            }
            else 
                perror("file open");
            return 0;
        }
        bzero(dir,sizeof(dir));
        bzero(file_num,sizeof(file_num));
        bzero(buff,sizeof(buff));
    }
    while((wait(NULL)) > 0); // wait for child

    clock_gettime(CLOCK_REALTIME,&end); // end measuring performance time
    //double time = (end.tv_sec - begin.tv_sec) + (end.tv_nsec - begin.tv_nsec);
	printf("Run time : %f\n",((end.tv_sec - begin.tv_sec) + (double)(end.tv_nsec - begin.tv_nsec)/1000000000));
    return 0;
}