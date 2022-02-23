#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/string.h>
#include <linux/mm.h>
#include <linux/syscalls.h>
#include <asm/unistd.h>
#include <asm/uaccess.h>

#define MY_FILE "/tmp/log.txt"

char buf[256];
loff_t pos = 0;
struct file *filep = NULL;

static int __init kernel_rw_init(void)
{
	int write_ret = 0;

	printk("Hello, I'm the module that intends to write messages to file.\n");

	if(filep == NULL) {
		filep = filp_open(MY_FILE, O_RDWR | O_APPEND | O_CREAT, 0644);
	}

	if (IS_ERR(filep)) {
		printk("Open file %s error\n", MY_FILE);
		return -1;
	}

	sprintf(buf,"%s\n", "This is test message!");

	memset(buf, 0, sizeof(buf));
	write_ret = kernel_write(filep, buf, strlen(buf), &pos);
	if (write_ret != strlen(buf)) {
		printk("Kernel write file failed\n");	
		filp_close(filep, NULL);
		return -1;
	}

	memset(buf, 0, sizeof(buf));
	kernel_read(filep, buf, sizeof(buf), &pos);

	printk("Read buf -> %s\n", buf);

	return 0;
}

static void __exit kernel_rw_exit(void)
{
	printk("Kernel read/write exit\n");

	if(filep != NULL) {
		filp_close(filep, NULL);
	}
}

module_init(kernel_rw_init);
module_exit(kernel_rw_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Curtis");
