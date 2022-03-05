#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>
#include <linux/slab.h>

#include "hlist.h"

static DEFINE_HASHTABLE(ps_info_hashtable, DEFAULT_HASH_BUCKET_BITS);
static rwlock_t ps_info_hashtable_lock;

static void operate_ps_hashtable(uint32_t ops)
{
	int bkt = 0;
	struct hlist_node *tmp = NULL;
	struct process_info *info = NULL;

	if (ops & QUERY_TAB) {
		read_lock(&ps_info_hashtable_lock);
		hash_for_each_safe(ps_info_hashtable, bkt, tmp, info, node) {
			printk("pid -> %d comm -> %s\n", info->pid, info->comm);
		}
		read_unlock(&ps_info_hashtable_lock);
	}

	if (ops & CLEAN_TAB) {
		read_lock(&ps_info_hashtable_lock);
		hash_for_each_safe(ps_info_hashtable, bkt, tmp, info, node) {
			hash_del(&info->node);
			kfree(info);
		}
		read_unlock(&ps_info_hashtable_lock);
	}
}

static void update_info(struct task_struct *tsk)
{
	uint32_t num_hash = 0;
	struct process_info *info = NULL;

	num_hash = jhash_num(tsk->pid);

	info = (struct process_info *)kmalloc(sizeof(struct process_info),GFP_KERNEL);
	if (info == NULL) {
		printk("Malloc memory failed\n");
	}

	info->pid = tsk->pid;
	strncpy(info->comm, tsk->comm, LIST_LENTH);

	write_lock(&ps_info_hashtable_lock);
	hash_add(ps_info_hashtable, &info->node, num_hash);
	write_unlock(&ps_info_hashtable_lock);
}

static int __init hlist_init(void)
{
	struct task_struct *tsk = NULL;

	rcu_read_lock();
	for_each_process(tsk) {
		if (tsk->pid > LIST_LENTH) {
			break;
		}
		
		update_info(tsk);
	}

	operate_ps_hashtable(QUERY_TAB);
	operate_ps_hashtable(CLEAN_TAB);

	return 0;
}

static void __exit hlist_exit(void)
{
	printk("Hlist example exit!\n");
}

module_init(hlist_init);
module_exit(hlist_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("curtis");
