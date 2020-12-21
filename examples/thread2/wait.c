#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>

#define MAX 5

int main()
{
	pid_t pid;
	int a,i;
	for(i=0; i<MAX; i++)
	{
		if((pid = fork()) < 0) return 1;
		else if(pid == 0) exit(i);
	}
	for(i=0; i<MAX; i++)
	{
		wait(&a);
		printf("original exit valiable : %d\n",a);
		printf("shift exit valiable : %d\n", a>>8);
	}
	return 0;
}
