#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>

/* 描述性定义 */
MODULE_LICENSE("GPL");                              // 模块的许可证声明
MODULE_AUTHOR("wzr");                               //模块作者声明
MODULE_DESCRIPTION("my first hello kernel module"); //模块描述声明
MODULE_VERSION("v1.0");                             //模块版本声明

static int hello_init(void)
{
    printk(KERN_NOTICE "hello kernel!\n");
    return 0;
}

static void hello_exit(void)
{
    printk(KERN_ALERT "bye kernel!\n");
}

module_init(hello_init); //注册到内核
module_exit(hello_exit);