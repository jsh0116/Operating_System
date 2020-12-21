/////////////////////////////////////////////////////////////////////////////
// File Name       : file_varea.c                                          //
// Date            : 2020/11/06 ~ 2020/11/29                               //                      
// OS              : Ubuntu 16.04.3 LTS                                    //
// Student Name    : Seung Hoon Jeong                                      //                 
// Student ID      : 2015707003                                            //                     
// ----------------------------------------------------------------------- //
// Title : OS_Assignment4                                                  //
// Description : print process information system call for module wrapping //
/////////////////////////////////////////////////////////////////////////////
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/kallsyms.h>
#include <linux/sched/mm.h>
#include <linux/mm.h>
#include <linux/syscalls.h>
#include <asm/syscall_wrapper.h> // __SYSCALL_DEFINEx()
#include <linux/highmem.h>
#define __x64_sys_add 349 // set "add" system call yourself
#define MAX_BUF 1024

void **syscall_table; // to store original "add" system call address pointer

asmlinkage pid_t (*real_add)(struct pt_regs *regs);

//////////////////////////////////////////////////////////////////////////////////////////////////
// Function : __SYSCALL_DEFINEx(1,info, ...                                                     //
// =============================================================================================//
// Input: x                                                                                     //
// Output: Print process information instead of the addition operation performed previously     //
// =============================================================================================//
// Purpose: Define info system call to replace the add system call                              //
//////////////////////////////////////////////////////////////////////////////////////////////////
__SYSCALL_DEFINEx(1,info,pid_t,pid)
{
	struct task_struct *t = NULL; // store task information
	struct mm_struct *mm = NULL; // data structure for holding information about memory which task uses
    	struct vm_area_struct *vma = NULL; // list of VMAs
	struct file *f = NULL; // mean file itself
	char *buff = kmalloc(MAX_BUF, GFP_KERNEL); // for kernel memory allocation
    	char *filename = NULL;
	
	/* return "task_struct" structure */
	if((t = pid_task(find_vpid(pid), PIDTYPE_PID)) == NULL) {
        	printk(KERN_ERR "pid_task error\n");
        	return 0;
    	}
	
	/* Increment use count 1, Bring "t" task_struct structure */
    	if((mm = get_task_mm(t)) == NULL) {
		printk(KERN_ERR "get_task_mm error\n");
		return 0;
	}

    	printk(KERN_INFO "########## Loaded files of a process '%s(%d)' in VM ##########",t->comm, t->pid); // Print pid and name existed in kernel
	vma = mm->mmap; // store list of VMAs.
    	while(vma) // until nowhere task.
    	{
        	f = vma->vm_file;  // point to file in virtual memory
		if(f)
		{
			memset(buff,0,MAX_BUF);
			filename = d_path(&f->f_path,buff,MAX_BUF); // save file's entire path at buffer
			/* Print virtual memory's start & end address,  
				code segment area,
				data segment area,
				heap's start, end,
				information's original file path.
			*/
            		printk(KERN_INFO "mem(%06lx-%06lx) code(%06lx-%06lx) data(%06lx-%06lx) heap(%07lx-%07lx) %s\n",
                		vma->vm_start,vma->vm_end,mm->start_code,mm->end_code,mm->start_data, mm->end_data,mm->start_brk,mm->brk,filename);
		}
		vma = vma->vm_next; // point to another virtual memory block
    	}
	printk(KERN_INFO "###########################################################################################################################\n");
    	mmput(mm); // decrement use count 1, if use count == 1 : free allocated memory space.
	kfree(buff); // kernel free()
	return 0;
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
 	pte_t *pte = lookup_address((u64)addr, &level); // page global directory's start address
	pte->pte = pte->pte &~ _PAGE_RW;
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
// Function : static int __init file_varea_init(void)                                 //
// ================================================================================== //
// Input: x                                                                           //
// Output:                                  //     
// ================================================================================== //
// Purpose: Perform initialization when module installed                              //
////////////////////////////////////////////////////////////////////////////////////////
static int __init file_varea_init(void)
{
	/* Find system call table */
	syscall_table = (void**) kallsyms_lookup_name("sys_call_table");

	/* Change permission of the page of system call table to both readable and writable */
	make_rw(syscall_table);
	real_add = syscall_table[__x64_sys_add]; // store original system call address when module is unlocked
	syscall_table[__x64_sys_add] = __x64_sysinfo; // Replace "add system call with "info" system call for hooking 	
	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////
// Function : static void __exit file_varea_exit(void)                                //
// ================================================================================== //
// Input: x                                                                           //
// Output: [current kernel time] [exit_MY_STUDENT_ID]                                 //     
// ================================================================================== //
// Purpose: Perform return operation when module uninstalled                          //
////////////////////////////////////////////////////////////////////////////////////////
static void __exit file_varea_exit(void)
{
	syscall_table[__x64_sys_add] = real_add; // restore the system call I hooked
	make_ro(syscall_table); // recover the page's permission (i.e. read-only)
	printk(KERN_INFO "[EXIT] [2015707003]\n"); // for check module unload
}

module_init(file_varea_init); // insmod
module_exit(file_varea_exit); // rmmod
MODULE_LICENSE("GPL");
