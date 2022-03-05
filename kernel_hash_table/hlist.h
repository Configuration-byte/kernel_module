#ifndef _LIST_H
#define _LIST_H

#include <linux/list.h>
#include <linux/hashtable.h>
#include <linux/jhash.h>

#define LIST_LENTH 10
#define QUERY_TAB 1
#define CLEAN_TAB (1 << 1)
#define DEFAULT_HASH_BUCKET_BITS 17
#define jhash_num(num) jhash((&num), sizeof(unsigned int), 0x95279527)

struct process_info {
	unsigned int pid;
	char comm[TASK_COMM_LEN];
	struct hlist_node node;
};

#endif	/* _LIST_H */ 
