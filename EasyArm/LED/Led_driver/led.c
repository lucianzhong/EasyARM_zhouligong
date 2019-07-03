/*
   GPIO Driver driver for EasyARM-iMX283
*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/ioctl.h>
#include <linux/delay.h>
#include <linux/bcd.h>
#include <linux/capability.h>
#include <linux/rtc.h>
#include <linux/cdev.h>
#include <linux/miscdevice.h>
#include <linux/gpio.h>

#include <../arch/arm/mach-mx28/mx28_pins.h>

#define DEVICE_NAME "imx28x_led"

#define LED_GPIO    MXS_PIN_TO_GPIO(PINID_LCD_D23)      //for 283 287A/B
//#define LED_GPIO  MXS_PIN_TO_GPIO(PINID_SAIF0_MCLK)   //for 280

/*--------------------------------------------------------------------------------------------------------
*/
static int gpio_open(struct inode *inode, struct file *filp);
static int  gpio_release(struct inode *inode, struct file *filp);
ssize_t gpio_write(struct file *filp, const char __user *buf, size_t count,
                loff_t *f_pos);
static int gpio_ioctl(struct inode *inode,struct file *flip,unsigned int command,unsigned long arg);
static int gpio_init(void);
static void gpio_exit(void);

/*--------------------------------------------------------------------------------------------------------
*/

static int gpio_open(struct inode *inode, struct file *filp)
{
    gpio_request(LED_GPIO, "led");

    return 0;
}

static int  gpio_release(struct inode *inode, struct file *filp)
{
    gpio_free(LED_GPIO);
    return 0;
}


ssize_t gpio_write(struct file *filp, const char __user *buf, size_t count,
                loff_t *f_pos)
{
    char data[2];

    copy_from_user(data, buf, count);
    gpio_direction_output(LED_GPIO, data[0]);

    return count;
}

static int gpio_ioctl(struct inode *inode,struct file *flip,unsigned int command,unsigned long arg)
{
    int data;
    
    switch (command) {
    case 0: 
        gpio_direction_output(LED_GPIO, 0);
        break;
    
    case 1:
        gpio_direction_output(LED_GPIO, 1);
        break;
    }

    return 0;
}

static struct file_operations gpio_fops={
    .owner      = THIS_MODULE,
    .open       = gpio_open,
    .write      = gpio_write,
    .release    = gpio_release,
    .ioctl      = gpio_ioctl,
};

static struct miscdevice gpio_miscdev = {
    .minor  = MISC_DYNAMIC_MINOR,
    .name   = DEVICE_NAME,
    .fops   = &gpio_fops,
};  

static int __init gpio_init(void)
{

    misc_register(&gpio_miscdev);
    printk(DEVICE_NAME" up. \n"); 

    return 0;
}

static void __exit gpio_exit(void)
{
    misc_deregister(&gpio_miscdev);
    printk(DEVICE_NAME " down.\n");
}

module_init(gpio_init);
module_exit(gpio_exit);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("zhuguojun, ZhiYuan Electronics Co, Ltd.");
MODULE_DESCRIPTION("GPIO DRIVER FOR EasyARM-i.MX28xx");


