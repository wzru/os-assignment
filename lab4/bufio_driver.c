#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("wzr");
MODULE_DESCRIPTION("my second linux driver bufio");
MODULE_VERSION("v1.0");

#define BUFIO_MAJOR 225
#define BUF_SIZE 64

/* 4 user */
static char *buf;
static int rp, wp;

/* open */
static int bufio_open(struct inode *inode, struct file *file)
{
    printk(KERN_ALERT "my bufio driver open()\n");
    rp = 0, wp = 0;
    return 0;
}

/* read */
static ssize_t bufio_read(struct file *h_file, char *h_buf, size_t h_count, loff_t *h_offset)
{
    int h_size;
    if (rp + h_count >= BUF_SIZE)
        h_size = BUF_SIZE - rp;
    else
        h_size = h_count;
    printk(KERN_ALERT "rp = %d, h_count = %d\n", rp, h_count);
    int h_re = copy_to_user(h_buf, buf + rp, h_size);
    printk(KERN_ALERT "read: buf = %s, h_size = %d\n", buf, h_size);
    rp += h_count;
    return h_size;
}

/* write */
static ssize_t bufio_write(struct file *h_file, const char *h_buf, size_t h_count, loff_t *h_offset)
{
    int h_size;
    if (wp + h_count >= BUF_SIZE)
        h_size = BUF_SIZE - wp;
    else
        h_size = h_count;
    printk(KERN_ALERT "wp = %d, h_count = %d\n", wp, h_count);
    int h_re = copy_from_user(buf + wp, h_buf, h_size);
    printk(KERN_ALERT "write: buf = %s, h_size = %d\n", buf, h_size);
    wp += h_count;
    return h_size;
}

/* init struct file_operations */
static struct file_operations bufio_fops = {
    .owner = THIS_MODULE,
    .open = bufio_open,
    .read = bufio_read,
    .write = bufio_write};

/* init */
static int bufio_init(void)
{
    int r = register_chrdev(BUFIO_MAJOR, "bufio_driver", &bufio_fops);
    if (r < 0)
    {
        printk("fail to register\n");
        return r;
    }
    buf = kmalloc(BUF_SIZE, GFP_KERNEL);
    printk(KERN_ALERT "bufio driver init success\n");
    return 0;
}
/* exit */
static void bufio_exit(void)
{
    unregister_chrdev(BUFIO_MAJOR, "bufio_driver");
    kfree(buf);
    printk(KERN_ALERT "bufio_exit !\n");
}

module_init(bufio_init);
module_exit(bufio_exit);