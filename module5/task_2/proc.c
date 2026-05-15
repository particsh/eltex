#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

#define PROC_NAME "hello" // имя файлы
#define BUFF_SIZE 256 // размер буффера


// сделали переменные статическим дабы не было конфликта имен
static int len;
static int temp;
static char *msg;
 
static ssize_t read_proc(struct file *filp, char *buf, size_t count, loff_t *offp ) {
    if(count > temp) {
        count = temp;
    }
    temp = temp - count;
    // проверяем на наличие пропущенных байтов
    if (copy_to_user(buf, msg, count) > 0){
        return -EFAULT;
    }
    if(count == 0)
        temp = len;
    return count;
}
 
static ssize_t write_proc(struct file *filp, const char *buf, size_t count, loff_t *offp) {
    // проверяем на наличие пропущенных байтов
    if (copy_from_user(msg, buf, count) > 0){
        return -EFAULT;
    }
    len = count;
    temp = len;
    return count;
}

// изменили под наше ядро
static const struct proc_ops proc_fops = {
    .proc_read = read_proc,
    .proc_write = write_proc,
};
 
static void create_new_proc_entry(void) { 
    proc_create(PROC_NAME, 0, NULL, &proc_fops);
    msg = kmalloc(BUFF_SIZE * sizeof(char), GFP_KERNEL);
}
 
static int proc_init (void) {
    create_new_proc_entry();
    return 0;
}

static void proc_cleanup(void) {
    remove_proc_entry(PROC_NAME, NULL);
    kfree(msg);
}
 
MODULE_LICENSE("SUPER LICENSE OF PARTICSH");
MODULE_AUTHOR("Плотницкий Максим");
module_init(proc_init);
module_exit(proc_cleanup);