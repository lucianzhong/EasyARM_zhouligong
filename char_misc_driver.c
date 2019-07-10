#include <linux/init.h> 
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#define DEVICE_NAME "char_misc"


 static int char_misc_open(struct inode *inode, struct file *file )
 {
 try_module_get(THIS_MODULE);
 printk(KERN_INFO DEVICE_NAME "opened!\n");
 return 0;
}

 static int char_misc_release(struct inode *inode, struct file *file )
 {
 printk(KERN_INFO DEVICE_NAME "closed!\n");
 module_put(THIS_MODULE);
 return 0;
}

 static ssize_t char_misc_read(struct file *file, char *buf,size_t count, loff_t *f_pos)
 {
 printk(KERN_INFO DEVICE_NAME "read method!\n");
 return count;
}

 static ssize_t char_misc_write(struct file *file, const char *buf, size_t count, loff_t *f_pos)
 {
 printk(KERN_INFO DEVICE_NAME "write method!\n");
 return count;

}
 static int char_misc_ioctl(struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg)
 {
 printk(KERN_INFO DEVICE_NAME "ioctl method!\n");
 return 0;
}

struct file_operations char_misc_fops = {
 .owner = THIS_MODULE,
 .read = char_misc_read,
 .write = char_misc_write,
 .open = char_misc_open,
 .release = char_misc_release,
 .ioctl = char_misc_ioctl
};

 /* misc 结构体定义和初始化 */
 static struct miscdevice char_misc = {
 .minor = MISC_DYNAMIC_MINOR,
 .name = DEVICE_NAME,
 .fops = &char_misc_fops,
};

 static int __init char_misc_init(void)
 {
int ret;

 ret = misc_register(&char_misc); /* 注册 misc 设备 */
 if (ret < 0) {
 printk(KERN_ERR "misc_register error!\n");
 return -1;
}
return 0;
}

 static void __exit char_misc_exit(void)
 {
misc_deregister(&char_misc); /* 卸载 misc 设备 */

}

 module_init(char_misc_init);
 module_exit(char_misc_exit);

 MODULE_LICENSE("GPL");
 MODULE_AUTHOR("Chenxibing, linux@zlgmcu.com");