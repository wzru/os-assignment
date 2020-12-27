#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("wzr");
MODULE_DESCRIPTION("my first linux driver");
MODULE_VERSION("v1.0");

#define DRIVER_MAJOR 224
#define DRIVER_BUF_SIZE 1024

/* 4 user */
static char buf[DRIVER_BUF_SIZE];
static int result;

/* open */
static int driver_open(struct inode *inode, struct file *file)
{
    printk(KERN_ALERT "my driver_open()\n");
    return 0;
}

/* read */
static ssize_t driver_read(struct file *h_file, char *h_buf, size_t h_count, loff_t *h_offset)
{
    int a = (int)buf[0], b = (int)buf[sizeof(int)];
    int offset = a > b ? 0 : sizeof(int);
    printk(KERN_ALERT "a = %d, b = %d, max = %d\n", a, b, result);
    int re = copy_to_user(h_buf, buf + offset, sizeof(int));
    return (sizeof(int) - re);
}

/* write */
static ssize_t driver_write(struct file *h_file, const char *h_buf, size_t h_count, loff_t *h_offset)
{
    int h_size = (h_count < sizeof(buf) ? h_count : sizeof(buf));
    int h_re = copy_from_user(buf, h_buf, h_size);
    printk(KERN_ALERT "my driver_write()\n");
    return (h_size - h_re);
}

/* init struct file_operations */
static struct file_operations driver_fops = {
    .owner = THIS_MODULE,
    .open = driver_open,
    .read = driver_read,
    .write = driver_write};

/* init */
static int driver_init(void)
{
    int r = register_chrdev(DRIVER_MAJOR, "hello_driver", &driver_fops);
    if (r < 0)
    {
        printk("fail to register hello_driver!\n");
        return r;
    }
    return 0;
}
/* exit */
static void driver_exit(void)
{
    unregister_chrdev(DRIVER_MAJOR, "hello_driver");
    printk(KERN_ALERT "my hello driver_exit()\n");
}

module_init(driver_init);
module_exit(driver_exit);