#include <linux/module.h>
#include <linux/init.h>

static int __init my_test_init(void)
{
	printk("hello world, LKM\n");
	return 0;
}

static void __exit my_test_exit(void)
{
	printk("goodbye\n");
}

module_init(my_test_init);
module_exit(my_test_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Curtis");
MODULE_DESCRIPTION("hello world module");
