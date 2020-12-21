////////////////////////////////////////////////
// File Name       : my_add.c                 //
// Date            : 2020/09/26 ~ 2020/10/22  // 
// OS              : Ubuntu 16.04.3 LTS       // 
// Student Name    : Seung Hoon Jeong         //
// Student ID      : 2015707003               //
// -------------------------------------------//
// Title : Operating System Assignment #2     //
// Description : system call define           //
////////////////////////////////////////////////
#include <linux/kernel.h>
#include <linux/syscalls.h>

SYSCALL_DEFINE2(add, int, a, int, b)
{
	long ret;
	ret = a+b;
	return ret;
}
