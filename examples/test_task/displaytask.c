#include <linux/module.h>
#include <linux/sched.h>
#include <linux/init_task.h>

int displaytask_init(void)
{
	struct task_struct *findtask = &init_task;
	do {
		printk("%s[%d] -> ", findtask->comm, findtask->pid);
		findtask = next_task(findtask);
	}while((findtask->pid != init_task.pid));
	printk("%s[%d]\n",findtask->comm,findtask->pid);
	return 0;
}

void displaytask_exit(void)
{
}

module_init(displaytask_init);
module_exit(displaytask_exit);
MODULE_LICENSE("GPL");
