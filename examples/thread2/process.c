#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#define MAX 5

void child();
void parent();

int main()
{
	pid_t pid;
	if((pid = fork()) < 0)
		return 1;
	else if(pid == 0)
		child();
	else
		parent();
	return 0;
}

void child()
{
	for(int i=0; i<MAX; ++i,sleep(1))
		printf("child %d\n", i);
	printf("child done.\n");
}

void parent()
{
	for(int i=0; i<MAX; ++i, sleep(1))
		printf("parent %d\n", i);
	printf("parent done.\n");
}
