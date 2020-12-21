/////////////////////////////////////////////////////////////////////////////
// File Name       : test.c                                                //
// Date            : 2020/11/06 ~ 2020/11/29                               //                      
// OS              : Ubuntu 16.04.3 LTS                                    //
// Student Name    : Seung Hoon Jeong                                      //                 
// Student ID      : 2015707003                                            //                     
// ----------------------------------------------------------------------- //
// Title : OS_Assignment4                                                  //
// Description : test file for checking when before and after wrapping     //
/////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <linux/unistd.h>

#define __x64_sys_add 349

int main(void)
{
	int a,b;
	a = 7;
	b = 4;

	#ifdef WRAPPING
		printf("%d add %d = %ld\n",a,b,syscall(__x64_sys_add, getpid()));
	#else
		printf("%d add %d = %ld\n",a,b,syscall(__x64_sys_add,a,b));
    	#endif    
    	return 0;
}
