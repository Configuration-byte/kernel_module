#include <linux/init.h>
#include <linux/module.h>
#include <linux/kallsyms.h>
#include <linux/slab.h>

static char *path = NULL;
static unsigned long base_addr = 0;
static unsigned int offset = 0;

module_param(path, charp, S_IRUGO);
module_param(base_addr, ulong, S_IRUGO);
module_param(offset, uint, S_IRUGO);

static int __write_to_file(char *buffer, int size)
{
	struct file *filep = NULL;
	loff_t pos = 0;
	int ret = 0;

	filep = filp_open(path, O_RDWR | O_CREAT, 0);
	if(filep == NULL) {
		printk("%s: Open file %s error\n", __func__, path);
		return 0;
	}

	ret = kernel_write(filep, buffer, size, &pos);
	if(ret < size) {
		printk("%s: Write file %s error\n", __func__, path);
		return 0;
	}

	filp_close(filep, NULL);

	return ret;
}

static int __init kallsyms_init(void)
{
	unsigned long stext, etext;
	char *ptr = NULL;
	char *buffer = NULL;
	int i = 0;

	stext = base_addr;
	etext = base_addr + offset;

	printk("%s: the _stext address is %lx\n", __func__, stext);
	printk("%s: the _stext address is %lx\n", __func__, etext);

	ptr = (char *)stext;

	buffer = kmalloc((offset + 100), GFP_KERNEL);
	if(buffer == NULL) {
		printk("%s: Alloc temp buffer memory failed\n", __func__);
		return 0;
	}

	for(i = 0; i < offset; i++) {
		buffer[i] = ptr[i];
	}

	__write_to_file(buffer, offset);

	kfree(buffer);
	return 0;
}

static void __exit kallsyms_exit(void)
{
	printk("%s: good bye\n", __func__);
}

module_init(kallsyms_init);
module_exit(kallsyms_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Curtis");
