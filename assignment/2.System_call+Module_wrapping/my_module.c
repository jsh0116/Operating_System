//////////////////////////////////////////////////////////
// File Name       : my_module.c                        //
// Date            : 2020/09/26 ~ 2020/10/22            //                      
// OS              : Ubuntu 16.04.3 LTS                 //
// Student Name    : Seung Hoon Jeong                   //                 
// Student ID      : 2015707003                         //                     
// -----------------------------------------            //
// Title : OS_Assignment2                               //
// Description : module programming for module wrapping //
//////////////////////////////////////////////////////////

#include <linux/module.h>
#include <linux/highmem.h>
#include <linux/kallsyms.h> // kallsyms_lookup_name()
#include <linux/syscalls.h> // __SYSCALL_DEFINEx()
#include <asm/syscall_wrapper.h> // __SYSCALL_DEFINEx()

#define __x64_sys_add 349

void **syscall_table; // pointer to store original 'add' system call address
void *real_add;

//////////////////////////////////////////////////////////////////////////////////////////////////
// Function : __SYSCALL_DEFINEx(2,sub, ...                                                      //
// =============================================================================================//
// Input: x                                                                                     //
// Output: Perform subtraction operation instead of the addition operation performed previously //
// ===========================================================================================  //
// Purpose: Define sub system call to replace the add system                                    //
//////////////////////////////////////////////////////////////////////////////////////////////////
__SYSCALL_DEFINEx(2,sub,int,a,int,b)
{
	printk("%d + %d\n",a,b);
	return a-b;
}

////////////////////////////////////////////////////////////////////
// Function : void make_rw(void *addr)                            //
// ============================================================== //
// Input: x                                                       //
// Output: x                                                      //     
// ============================================================== //
// Purpose: give permission to write-prohibited system call table //
////////////////////////////////////////////////////////////////////
void make_rw(void *addr)
{
	unsigned int level;
	pte_t *pte = lookup_address((u64)addr, &level);
	if(pte->pte &~ _PAGE_RW)
		pte->pte |= _PAGE_RW;
}

////////////////////////////////////////////////////////////////////////////////////////
// Function : void make_ro(void *addr)                                                //
// ================================================================================== //
// Input: x                                                                           //
// Output: x                                                                          //     
// ================================================================================== //
// Purpose: Revoke read and write permission to the page to which the address belongs //
////////////////////////////////////////////////////////////////////////////////////////
void make_ro(void *addr)
{
	unsigned int level;
 	pte_t *pte = lookup_address((u64)addr, &level);
	pte->pte = pte->pte &~ _PAGE_RW;
}

////////////////////////////////////////////////////////////////////////////////////////
// Function : static int __init my_module_init(void)                                  //
// ================================================================================== //
// Input: x                                                                           //
// Output: [current kernel time] [init_MY_STUDENT_ID]                                 //     
// ================================================================================== //
// Purpose: Perform initialization when module installed                              //
////////////////////////////////////////////////////////////////////////////////////////
static int __init my_module_init(void)
{
	/* Find system call table */
	syscall_table = (void**) kallsyms_lookup_name("sys_call_table");

	/* Change permission of the page of system call table to both readable and writable */
	make_rw(syscall_table);
	real_add = syscall_table[__x64_sys_add]; // store original system call address when module is unlocked
	syscall_table[__x64_sys_add] = __x64_syssub; // Replace "add system call with "sub" system call for hooking 
	printk(KERN_INFO "init_[2015707003]\n");
	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////
// Function : static int __exit my_module_exit(void)                                  //
// ================================================================================== //
// Input: x                                                                           //
// Output: [current kernel time] [exit_MY_STUDENT_ID]                                 //     
// ================================================================================== //
// Purpose: Perform return operation when module uninstalled                          //
////////////////////////////////////////////////////////////////////////////////////////
static void __exit my_module_exit(void)
{
	syscall_table[__x64_sys_add] = real_add; // restore the system call I hooked
	make_ro(syscall_table); // recover the page's permission (i.e. read-only)
	printk(KERN_INFO "exit_[2015707003]\n");
}

module_init(my_module_init); // insmod or modprobe
module_exit(my_module_exit); // rmmod
MODULE_LICENSE("GPL");
