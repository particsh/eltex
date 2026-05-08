#include <linux/module.h>
#include <linux/init.h>
#include <linux/tty.h>          /* For fg_console, MAX_NR_CONSOLES */
#include <linux/kd.h>           /* For KDSETLED */
#include <linux/vt.h>
#include <linux/console_struct.h>       /* For vc_cons */
#include <linux/vt_kern.h>
#include <linux/timer.h>
#include <linux/printk.h> 
#include <linux/kobject.h> 
#include <linux/sysfs.h> 

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Плотницкий Максим");
MODULE_DESCRIPTION("blink keyboard by write in file");

static struct kobject *example_kobject;
static unsigned int sysfs_info;

struct timer_list my_timer;
struct tty_driver *my_driver;;
static int led_mask = 0;
#define BLINK_DELAY   HZ/5
#define RESTORE_LEDS  0xFF
#define FILE_NAME "sys_communicate"

static void my_timer_func(struct timer_list *ptr)
{
    static int toggle = 0;
    toggle = !toggle; // меняем значения каждым вызовоом

    int state = toggle ? led_mask : RESTORE_LEDS; // в зависимости от состояния включаем выключаем лампочки

        (my_driver->ops->ioctl) (vc_cons[fg_console].d->port.tty, KDSETLED,
                            state);
        my_timer.expires = jiffies + BLINK_DELAY;
        add_timer(&my_timer);
}

static ssize_t foo_show(struct kobject *kobj, struct kobj_attribute *attr,
                      char *buf)
{
        return sprintf(buf, "%d\n", sysfs_info);
}
 
static ssize_t foo_store(struct kobject *kobj, struct kobj_attribute *attr,
                      const char *buf, size_t count)
{
        sscanf(buf, "%u", &sysfs_info);
        led_mask = sysfs_info & 0x07; // применяем маску дабы получить нужный формат
        del_timer(&my_timer); // удаляем на всякий старый таймер

        if (led_mask != 0){ // если не ноль - включаем таймер 
                timer_setup(&my_timer, my_timer_func, 0);
                my_timer.expires = jiffies + BLINK_DELAY;
                add_timer(&my_timer);
        } else{ // иначе выключаем лампочки
                (my_driver->ops->ioctl) (vc_cons[fg_console].d->port.tty, KDSETLED, RESTORE_LEDS);   
        }

        return count;
}
 
 
static struct kobj_attribute foo_attribute =__ATTR(test, 0660, foo_show,
                                                   foo_store);


static int __init blink_keyboard_sysfs_init(void){
        int error = 0;
 
        pr_debug("Module initialized successfully \n");
 
        example_kobject = kobject_create_and_add(FILE_NAME,
                                                 kernel_kobj);
        if(!example_kobject)
                return -ENOMEM;
 
        error = sysfs_create_file(example_kobject, &foo_attribute.attr);
        if (error) {
                pr_debug("failed to create the foo file in /sys/kernel/%s \n", FILE_NAME);
        }
        int i;
        printk(KERN_INFO "kbleds: loading\n");
        printk(KERN_INFO "kbleds: fgconsole is %x\n", fg_console);
        for (i = 0; i < MAX_NR_CONSOLES; i++) {
                if (!vc_cons[i].d)
                        break;
                printk(KERN_INFO "poet_atkm: console[%i/%i] #%i, tty %lx\n", i,
                       MAX_NR_CONSOLES, vc_cons[i].d->vc_num,
                       (unsigned long)vc_cons[i].d->port.tty);
        }
        printk(KERN_INFO "kbleds: finished scanning consoles\n");
        my_driver = vc_cons[fg_console].d->port.tty->driver;
        printk(KERN_INFO "kbleds: tty driver found\n");

        return error;

}

static void __exit blink_keyboard_sysfs_exit(void){
        pr_debug ("Module uninitialized successfully \n");
        kobject_put(example_kobject);
        printk(KERN_INFO "kbleds: unloading...\n");
        del_timer(&my_timer);
        (my_driver->ops->ioctl) (vc_cons[fg_console].d->port.tty, KDSETLED,
                            RESTORE_LEDS);

}

module_init(blink_keyboard_sysfs_init);
module_exit(blink_keyboard_sysfs_exit);
