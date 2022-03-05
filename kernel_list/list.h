#ifndef _LIST_H
#define _LIST_H

#include <linux/list.h>

#define LIST_LENTH 10

struct process_info {
    unsigned int pid;
    char comm[TASK_COMM_LEN];
    struct list_head p_list;
};

#endif	/* _LIST_H */ 
