//////////////////////////////////////////////////////////////
// File Name       : gen_files.c                            //
// Date            : 2020/10/23 ~ 2020/11/05                //
// OS              : Ubuntu 16.04.3 LTS                     //
// Student Name    : Seung Hoon Jeong                       //
// Student ID      : 2015707003                             //
// -----------------------------------------                //
// Title : OS_Assignment3-2                                 //
// Description : random integer text file write on directory//
//////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <linux/sched.h>

#define MAX_PROCESSES 10000
#define MAX_BUFF 1024

int main(void)
{
	struct stat st = {0};
    	char dir[MAX_BUFF] = {0,};
    	char file_num[MAX_BUFF] = {0,};

    	if(stat("./tmp1", &st) == -1)
        	mkdir("./tmp1", 0777); // make directory

    	for(int i=0; i<MAX_PROCESSES; i++)
    	{
        	getcwd(dir,MAX_BUFF); // get current working directory
        	sprintf(file_num,"%d",i);
        	strcat(dir,"/tmp1/");
        	strcat(dir,file_num);
        	FILE *f_write = fopen(dir,"w"); // open file
        	if(f_write == NULL) {
            		perror("file open");
            		exit(1);
        	}
        	fprintf(f_write, "%d", (int)1+rand()%9); // write random number
        	fclose(f_write); // close file
        	bzero(dir,sizeof(dir));
    	}
	return 0;
}