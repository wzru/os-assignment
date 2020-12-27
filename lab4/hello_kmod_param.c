#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("wzr");
MODULE_DESCRIPTION("my first hello kernel module with parameter");
MODULE_VERSION("v1.0");

static char *msg;
module_param(msg, charp, 0644); //0644 用户拥有读写权限，组用户和其他用户只读

static int hello_param_init(void)
{
	printk(KERN_ALERT "hello kernel!\n");
	printk(KERN_ALERT "%s\n", msg);
	return 0;
}

static void hello_param_exit(void)
{
	printk(KERN_ALERT "bye kernel!\n");
	printk(KERN_ALERT "%s\n", msg);
}

module_init(hello_param_init);
module_exit(hello_param_exit);