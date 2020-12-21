#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#define MAX 5

int main()
{
	int pid1, pid2, status;
	pid_t child_pid;
	int a = 0, b = 0;
	
	if((pid1 = fork()) < 0) perror("fork");
	else if(pid1 == 0) {
		a = 1;
		printf("child_pid1 = %d\n", getpid());
		if((pid2 = fork()) == 0) {
			a = 2;
			printf("child_pid2 = %d\n\n", getpid());
			exit(a);
		}
		else {
			child_pid = waitpid(pid2,&status,0);
			printf("child_pid : %d\n",child_pid);
			printf("original status : %d\n",status);
			printf("shift status : %d\n\n", status>>8);
			exit(a);
		}
	}
	else {
		child_pid = waitpid(pid1,&status,0);
                printf("child_pid : %d\n",child_pid);
                printf("original status : %d\n",status);
                printf("shift status : %d\n\n", status>>8);
	}
	return 0;
}
