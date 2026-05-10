#include <linux/cdev.h> 
#include <linux/delay.h> 
#include <linux/device.h> 
#include <linux/fs.h> 
#include <linux/init.h> 
#include <linux/irq.h> 
#include <linux/kernel.h> 
#include <linux/module.h> 
#include <linux/poll.h> 
 

// объявляем протитпы функций
static int device_open(struct inode *, struct file *); 
static int device_release(struct inode *, struct file *); 
static ssize_t device_read(struct file *, char __user *, size_t, loff_t *); 
static ssize_t device_write(struct file *, const char __user *, size_t, loff_t *); 

// нужные дефайны, имя файла, длина буффера
#define SUCCESS 0 
#define DEVICE_NAME "chardev"  
#define BUF_LEN 80 

// старшое число устройства 
static int major; 

// состояния отрытия файла
enum { 
    CDEV_NOT_USED = 0, 
    CDEV_EXCLUSIVE_OPEN = 1, 
}; 
 

static atomic_t already_open = ATOMIC_INIT(CDEV_NOT_USED);  // состояние файл
static char msg[BUF_LEN]; // буффер
static struct class *cls; 
static short readPos = 0; // позиция в файле

// функции для взамодействия с файлом
static struct file_operations chardev_fops = {
    .owner = THIS_MODULE, // счетчик открытия файла
    .read = device_read, 
    .write = device_write, 
    .open = device_open, 
    .release = device_release, 
}; 

// инизиализация модуля
static int __init chardev_init(void) { 
    major = register_chrdev(0, DEVICE_NAME, &chardev_fops); 
 
    if (major < 0) { 
        pr_alert("Registering char device failed with %d\n", major); 
        return major; 
    } 

    pr_info("I was assigned major number %d.\n", major); 
 
    cls = class_create(DEVICE_NAME);
    // добавил защиту от ошибок, возможно это избычточно в контексте задания
    if (IS_ERR(cls)) {
        unregister_chrdev(major, DEVICE_NAME);
        pr_alert("Failed to create class\n");
        return PTR_ERR(cls);
    }

    struct device *dev = device_create(cls, NULL, MKDEV(major, 0), NULL, DEVICE_NAME); 
    // добавил защиту от ошибок, возможно это избычточно в контексте задания
    if (IS_ERR(dev)) {
        class_destroy(cls);
        unregister_chrdev(major, DEVICE_NAME);
        pr_alert("Failed to create device\n");
        return PTR_ERR(dev);
    }
    pr_info("Device created on /dev/%s\n", DEVICE_NAME); 
    return SUCCESS; 
} 

// выгрузка модуля
static void __exit chardev_exit(void){ 
    device_destroy(cls, MKDEV(major, 0));
    // добавил лог об удалении устройства
    pr_info("Device destroyed on /dev/%s\n", DEVICE_NAME); 
    class_destroy(cls); 
    unregister_chrdev(major, DEVICE_NAME); 
} 

// открытие модуля
static int device_open(struct inode *inode, struct file *file){ 
    static int counter = 0; 
    // чтобы только один процесс одновременно имел доступ к драйверу
    if (atomic_cmpxchg(&already_open, CDEV_NOT_USED, CDEV_EXCLUSIVE_OPEN)) 
        return -EBUSY; 
    pr_info("I already told you %d times Hello world!\n", counter++);
    return SUCCESS; 
} 

// закрытие модуля
static int device_release(struct inode *inode, struct file *file){ 
    atomic_set(&already_open, CDEV_NOT_USED); 
    return SUCCESS; 
} 

// чтение
static ssize_t device_read(struct file *filp,  
                           char __user *buffer, 
                           size_t length,
                           loff_t *offset) {
    short count = 0;
    while (length && readPos < BUF_LEN && msg[readPos] != 0) {
        if (put_user(msg[readPos], buffer++)) {
            return -EFAULT;
        }
        count++;
        length--;
        readPos++;
    }
    return count;
} 

// запись
static ssize_t device_write(struct file *filp, const char __user *buff, 
                            size_t len, loff_t *off){ 
    if (len > BUF_LEN - 1)      
        len = BUF_LEN - 1;

    if (copy_from_user(msg, buff, len)) {
        return -EFAULT;
    }

    msg[len] = '\0'; 
    readPos = 0;

    return len;
 
} 
 
module_init(chardev_init); 
module_exit(chardev_exit); 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Плотницкий Максим");
MODULE_DESCRIPTION("chardev <-> userspace");