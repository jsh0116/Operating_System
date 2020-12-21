#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>

int main(void)
{
	int a,b;
	long ret;
	
	a = 7;
	b = 3;
	
	ret = syscall(349,a,b);
	printf("%d op. %d = %ld\n", a, b, ret);

	return 0;
}
