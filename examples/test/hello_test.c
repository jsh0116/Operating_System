#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>

int main(void)
{
	long ret;
	ret = syscall(548);
	printf("%ld\n", ret);
	return 0;
}
