#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>
#include <linux/slab.h>

#include "list.h"

LIST_HEAD(p_head);
spinlock_t p_lock;

static void query_process_info(void)
{
	struct process_info *info, *tmp = NULL;

	spin_lock(&p_lock);
	list_for_each_entry_safe(info, tmp, &p_head, p_list) {
		printk("pid -> %d comm -> %s\n", info->pid, info->comm);
	}
	spin_unlock(&p_lock);
}

static void free_list(void)
{
	struct process_info *info, *tmp = NULL;

	spin_lock(&p_lock);
	list_for_each_entry_safe(info, tmp, &p_head, p_list) {
		list_del(&info->p_list);
		kfree(info);
	}
	spin_unlock(&p_lock);
}

static int __init list_init(void)
{

	struct task_struct *tsk = NULL;
	struct process_info *info = NULL;

	spin_lock_init(&p_lock);
	rcu_read_lock();
	for_each_process(tsk) {
		if (tsk->pid > LIST_LENTH) {
			break;
		}

		info = (struct process_info *)kmalloc(sizeof(struct process_info),GFP_KERNEL);
		if (info == NULL) {
			printk("Malloc memory failed\n");	
			break;
		}

		info->pid = tsk->pid;
		strncpy(info->comm, tsk->comm, LIST_LENTH);

		spin_lock(&p_lock);
		list_add_tail(&info->p_list, &p_head);
		spin_unlock(&p_lock);
	}

	query_process_info();
	free_list();

	return 0;
}

static void __exit list_exit(void)
{
	printk("List example exit!\n");
}

module_init(list_init);
module_exit(list_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("curtis");
