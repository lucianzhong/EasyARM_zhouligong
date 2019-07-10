 #include <linux/init.h>
 #include <linux/module.h>
 #include <linux/fs.h>
 #include <linux/cdev.h>
 #include <linux/device.h>

 static int major = 232; /* 静态设备号方式的默认值 */
 static int minor = 0; /* 静态设备号方式的默认值 */
 module_param(major, int, S_IRUGO);
 module_param(minor, int, S_IRUGO);

 struct cdev *char_cdev; /* cdev 数据结构 */
 static dev_t devno; /* 设备编号 */
 static struct class *char_cdev_class;

#define DEVICE_NAME "char_cdev"
// 驱动 open 方法
 static int char_cdev_open(struct inode *inode, struct file *file )
 {
 try_module_get(THIS_MODULE);  // try_module_get()用于增加模块引用计数,设备每被打开 1 次,模块引用计数加 1
 printk(KERN_INFO DEVICE_NAME " opened!\n");
 return 0;

}
 static int char_cdev_release(struct inode *inode, struct file *file )  //驱动 release 方法的实现代码
 {
 printk(KERN_INFO DEVICE_NAME " closed!\n");
 module_put(THIS_MODULE); // module_put()用于递减模块引用计数,设备被关闭 1 次,模块引用计数减 1;当引用计数为 0 时,模块可以被卸载
 return 0;
}
// 驱动 read 方法的实现代码
 static ssize_t char_cdev_read(struct file *file, char *buf,size_t count, loff_t *f_pos)
 {
 printk(KERN_INFO DEVICE_NAME " read method!\n");
 return count;
}
// 驱动 write 方法的实现代码
 static ssize_t char_cdev_write(struct file *file, const char *buf, size_t count, loff_t *f_pos)
 {
printk(KERN_INFO DEVICE_NAME " write method!\n");
return count;
}
// 驱动 ioctl 方法的实现代码
 static int char_cdev_ioctl(struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg)
 { 
 printk(KERN_INFO DEVICE_NAME " ioctl method!\n");
 return 0;
}
// 驱动 fops 的定义
struct file_operations char_cdev_fops = {
 .owner = THIS_MODULE,
 .read = char_cdev_read,
 .write = char_cdev_write,
 .open = char_cdev_open,
 .release = char_cdev_release,
 .ioctl = char_cdev_ioctl
};

 static int __init char_cdev_init(void)
 {

int ret;
devno = MKDEV(major, minor);/* 静态设备号 */
if (major > 0) {
ret = register_chrdev_region(devno, 1, "char_cdev");/* 动态设备号 */
} else {
 ret = alloc_chrdev_region(&devno, minor, 1, "char_cdev"); /* 从系统获取主设备号 */
 major = MAJOR(devno);
 }
 if (ret < 0) {
 printk(KERN_ERR "cannot get major %d \n", major);
 return -1;
}

 char_cdev = cdev_alloc();
 if (char_cdev != NULL) {  /* 分配 char_cdev 结构 */
 cdev_init(char_cdev, &char_cdev_fops);  // 通过 cdev_init()将 fops 与设备相关联
 char_cdev->owner = THIS_MODULE;
 if (cdev_add(char_cdev, devno, 1) != 0) {  ///* 增加 char_cdev 到系统中 */
 printk(KERN_ERR "add cdev error!\n");
 goto error;
}
} else {
printk(KERN_ERR "cdev_alloc error!\n"); /* 初始化 char_cdev 结构 */
return -1;
}

 char_cdev_class = class_create(THIS_MODULE, "char_cdev_class");
 if (IS_ERR(char_cdev_class)) {
 printk(KERN_INFO "create class error\n");
 return -1;
}

 //device_create(char_cdev_class, NULL, devno, NULL, "char_cdev" "%d", MINOR(devno));
 device_create(char_cdev_class, NULL, devno, NULL, "char_cdev", NULL);
 return 0;

error:
unregister_chrdev_region(devno, 1);  /* 释放已经获得的设备号 */
return ret;
}

 static void __exit char_cdev_exit(void)
 {
 cdev_del(char_cdev); /* 移除字符设备 */
 unregister_chrdev_region(devno, 1); /* 释放设备号 */
 device_destroy(char_cdev_class, devno); 
 class_destroy(char_cdev_class);

}

 module_init(char_cdev_init);
 module_exit(char_cdev_exit);

 MODULE_LICENSE("GPL");
 MODULE_AUTHOR("Chenxibing, linux@zlgmcu.com");











