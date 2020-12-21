//////////////////////////////////////////////
// File Name       : test_add.c             //                           
// Date            : 2020/09/26 ~ 2020/10/22//                           
// OS              : Ubuntu 16.04.3 LTS     //         
// Student Name    : Seung Hoon Jeong       //                 
// Student ID      : 2015707003             //                     
// -----------------------------------------//
// Title : OS_Assignment2                   //
// Description : System Call implementation //             
//////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/syscall.h>

int main(int argc, char *argv[])
{
	/* input argument setting */	
	if(argc != 3) {
		printf("Usage : ./[makefile] [num1] [num2]\n");
		exit(1);
	}
	/* if argument setting correct */
	else {
		int a,b;
		a = atoi(argv[1]);
		b = atoi(argv[2]);
		long ret = syscall(349,a,b);
        	printf("%d + %d = %ld\n",a,b,ret);
	}
	return 0;
}
