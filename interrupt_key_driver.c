#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/gpio.h>

#define KEY_GPIO 70 /* GPIO2_6 */
#define KEY_GPIO_IRQ  gpio_to_irq(KEY_GPIO) /* 中断号 */
#define DEVICE_NAME "key_irq" 16 struct cdev *key_irq; /* cdev 数据结构 */
static dev_t devno; /* 设备编号 */
static int major;
static int minor;


static struct class *key_irq_class;

 char const irq_types[5] = {
 IRQ_TYPE_EDGE_RISING,
 IRQ_TYPE_EDGE_FALLING,
 IRQ_TYPE_EDGE_BOTH,
 IRQ_TYPE_LEVEL_HIGH,
 IRQ_TYPE_LEVEL_LOW
 };

 static int key_irq_open(struct inode *inode, struct file *file )
 {
 try_module_get(THIS_MODULE);
 printk(KERN_INFO DEVICE_NAME " opened!\n");
 return 0;
 }

 static int key_irq_release(struct inode *inode, struct file *file )
 {
 printk(KERN_INFO DEVICE_NAME " closed!\n");
 module_put(THIS_MODULE);
 return 0;
 }

 static irqreturn_t key_irq_irq_handler (unsigned int irq, void *dev_id)
 {
 printk("KEY IRQ HAPPENED!\n");
 return IRQ_HANDLED;
 }

 struct file_operations key_irq_fops = {
 .owner = THIS_MODULE,
 .open = key_irq_open,
 .release = key_irq_release,
 };

 static int __init key_irq_init(void)
 {
int ret;

 gpio_free(KEY_GPIO);
 ret = gpio_request_one(KEY_GPIO, GPIOF_IN, "KEY IRQ"); /* 申请 IO */ 
 if (ret < 0) {
printk(KERN_ERR "Failed to request GPIO for KEY\n");
}
 gpio_direction_input(KEY_GPIO);   /* 设置 GPIO 为输入 */
 if (request_irq(KEY_GPIO_IRQ, key_irq_irq_handler, IRQF_DISABLED, "key_irq irq", NULL) )
{ /* 申请中断 */
printk(KERN_WARNING DEVICE_NAME": Can't get IRQ: %d!\n", KEY_GPIO_IRQ);
 }
 set_irq_type(KEY_GPIO_IRQ, irq_types[1]);
 disable_irq(KEY_GPIO_IRQ);
 enable_irq(KEY_GPIO_IRQ);

 ret = alloc_chrdev_region(&devno, minor, 1, DEVICE_NAME); /* 从系统获取主设备号 */
 major = MAJOR(devno);
 if (ret < 0) {
 printk(KERN_ERR "cannot get major %d \n", major);
 return -1;
}

 key_irq = cdev_alloc();  /* 分配 key_irq 结构 */
 if (key_irq != NULL) {
 cdev_init(key_irq, &key_irq_fops);  /* 初始化 key_irq 结构 */
 key_irq->owner = THIS_MODULE;
 if (cdev_add(key_irq, devno, 1) != 0) {  /* 增加 key_irq 到系统中 */
 printk(KERN_ERR "add cdev error!\n");
 goto error;
}
} else {
 printk(KERN_ERR "cdev_alloc error!\n");
 return -1;
}

 key_irq_class = class_create(THIS_MODULE, "key_irq_class");
 if (IS_ERR(key_irq_class)) {
 printk(KERN_INFO "create class error\n");
 return -1;
}

 device_create(key_irq_class, NULL, devno, NULL, DEVICE_NAME);
 return 0;

 error:
 unregister_chrdev_region(devno, 1);  /* 释放已经获得的设备号 */
 return ret;
 }
 static void __exit key_irq_exit(void)
 {
 gpio_free(KEY_GPIO); 109 disable_irq(KEY_GPIO_IRQ); 110 free_irq(KEY_GPIO_IRQ, NULL); 111 cdev_del(key_irq); /* 移除字符设备 */
 unregister_chrdev_region(devno, 1); /* 释放设备号 */
 device_destroy(key_irq_class, devno); 114 class_destroy(key_irq_class);
 }

 module_init(key_irq_init);
 module_exit(key_irq_exit);

 MODULE_LICENSE("GPL");

 MODULE_AUTHOR("Chenxibing, linux@zlgmcu.com");


