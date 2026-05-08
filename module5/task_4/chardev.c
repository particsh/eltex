#include <linux/module.h>
#include <linux/string.h>
#include <linux/fs.h>
#include <linux/uaccess.h>  
#include <linux/printk.h>    

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("chardev task"); 
MODULE_AUTHOR("Плотницкий Максим");

static char msg[100] = {0};
static short readPos = 0;
static int times = 0;

static int dev_open(struct inode *, struct file *);
static int dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char __user *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char __user *, size_t, loff_t *);

static struct file_operations fops = {
    .read    = dev_read,
    .open    = dev_open,
    .write   = dev_write,
    .release = dev_release,
};


static int __init init_module(void)
{
    int t = register_chrdev(80, "myDev", &fops);
    if (t < 0)
        printk(KERN_ALERT "Device registration failed..\n");
    else
        printk(KERN_ALERT "Device registered...\n");
    return t;
}

static void __exit cleanup_module(void)
{
    unregister_chrdev(80, "myDev");
    printk(KERN_ALERT "Device unregistered\n");
}

static int dev_open(struct inode *inod, struct file *fil)
{
    times++;
    printk(KERN_ALERT "Device opened %d times\n", times);
    return 0;
}

static ssize_t dev_read(struct file *filp, char __user *buff, size_t len, loff_t *off)
{
    short count = 0;
    while (len && readPos < 100 && msg[readPos] != 0) {
        if (put_user(msg[readPos], buff++)) {
            return -EFAULT;
        }
        count++;
        len--;
        readPos++;
    }
    return count;
}

static ssize_t dev_write(struct file *filp, const char __user *buff, size_t len, loff_t *off)
{
    short count = 0;
    short ind;          
    char kernel_buf[100] = {0};

    if (len > 99)       
        len = 99;

    if (copy_from_user(kernel_buf, buff, len)) {
        return -EFAULT;
    }

    memset(msg, 0, sizeof(msg));
    readPos = 0;

    for (ind = len - 1; ind >= 0; ind--) {
        msg[count++] = kernel_buf[ind];
    }
    msg[count] = '\0';

    return count;
}

static int dev_release(struct inode *inod, struct file *fil)
{
    printk(KERN_ALERT "Device closed\n");
    return 0;
}

module_init(init_module);
module_exit(cleanup_module);