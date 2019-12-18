//此文件创建led的字符设备，包括驱动，设备和设备节点

/*包含初始化宏定义的头文件,代码中的module_init和module_exit在此文件中*/
#include <linux/init.h>
/*包含初始化加载模块的头文件,代码中的MODULE_LICENSE在此头文件中*/
#include <linux/module.h>
/*定义module_param module_param_array的头文件*/
#include <linux/moduleparam.h>
/*定义module_param module_param_array中perm的头文件*/
#include <linux/stat.h>
/*三个字符设备函数*/
#include <linux/fs.h>
/*MKDEV转换设备号数据类型的宏定义*/
#include <linux/kdev_t.h>
/*定义字符设备的结构体*/
#include <linux/cdev.h>
/*分配内存空间函数头文件*/
#include <linux/slab.h>
/*包含函数device_create 结构体class等头文件*/
#include <linux/device.h>

/*自定义头文件*/
#include "led_character_driver.h"

/*Linux中申请GPIO的头文件*/
#include <linux/gpio.h>
/*三星平台的GPIO配置函数头文件*/
/*三星平台EXYNOS系列平台，GPIO配置参数宏定义头文件*/
#include <plat/gpio-cfg.h>
/*三星平台4412平台，GPIO宏定义头文件*/
#include <mach/gpio-exynos4.h>

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Holden");

// LED2 KPCOL0 GPL2_0
// LED3 VDD50_EN GPK1_1
static int led_gpios[] = {
    EXYNOS4_GPL2(0),
    EXYNOS4_GPK1(1),
};
#define LED_CNT ARRAY_SIZE(led_gpios)

int numdev_major = DEV_MAJOR;
int numdev_minor = DEV_MINOR;

/*模块参数，主设备号*/
module_param(numdev_major, int, S_IRUSR);
/*模块参数，次设备号*/
module_param(numdev_minor, int, S_IRUSR);

static struct class *myclass;
struct reg_dev *led_devices;

// driver
/*打开操作*/
static int led_fops_open(struct inode *inode, struct file *file) {
    printk(KERN_EMERG "led_fops_open is success!\n");

    return 0;
}

/*关闭操作*/
static int led_fops_release(struct inode *inode, struct file *file) {
    printk(KERN_EMERG "led_fops_release is success!\n");

    return 0;
}

/*IO操作*/
// cmd 选择高低电平，arg=0选择LED2，arg=1选择LED3
static long led_fops_ioctl(struct file *file, unsigned int cmd,
                           unsigned long arg) {

    switch (cmd) {
    case 0:
    case 1:
        if (arg > LED_CNT) {
            return -EINVAL;
        }

        gpio_set_value(led_gpios[arg], cmd);
        break;

    default:
        return -EINVAL;
    }

    printk(KERN_EMERG "led_fops_ioctl is success! cmd is %d,arg is %d \n", cmd,
           arg);

    return 0;
}

ssize_t led_fops_read(struct file *file, char __user *buf, size_t count,
                      loff_t *f_ops) {
    return 0;
}

ssize_t led_fops_write(struct file *file, const char __user *buf, size_t count,
                       loff_t *f_ops) {
    return 0;
}

loff_t led_fops_llseek(struct file *file, loff_t offset, int ence) { return 0; }

struct file_operations led_fops = {
    .owner = THIS_MODULE,
    .open = led_fops_open,
    .release = led_fops_release,
    .unlocked_ioctl = led_fops_ioctl,
    .read = led_fops_read,
    .write = led_fops_write,
    .llseek = led_fops_llseek,
};
#if 0
// device
/*设备注册到系统*/
static void reg_init_cdev(struct reg_dev *dev, int index) {
    int err;
    int devno = MKDEV(numdev_major, numdev_minor + index);

    /*数据初始化*/
    cdev_init(&dev->cdev, &led_fops);
    dev->cdev.owner = THIS_MODULE;
    dev->cdev.ops = &led_fops;

    /*设备节点注册到系统*/
    err = cdev_add(&dev->cdev, devno, 1);

    if (err) {
        printk(KERN_EMERG "cdev_add %d is fail! %d\n", index, err);
    } else {
        printk(KERN_EMERG "cdev_add %d is success!\n", numdev_minor + index);
    }
}
#endif
static int gpio_init(void) {
    int i = 0, ret;

    for (i = 0; i < LED_CNT; i++) {
        ret = gpio_request(led_gpios[i], "LED_CHARACTER_DRIVER");

        if (ret) {
            printk("%s: request GPIO %d for LED failed, ret = %d\n",
                   DEVICE_NAME, i, ret);
            return -1;
        } else {
            s3c_gpio_cfgpin(led_gpios[i], S3C_GPIO_OUTPUT);
            gpio_set_value(led_gpios[i], 1);
        }
    }

    return 0;
}

static int scdev_init(void) {
    int ret = 0, i;
    dev_t num_dev;

    printk(KERN_EMERG "numdev_major is %d!\n", numdev_major);
    printk(KERN_EMERG "numdev_minor is %d!\n", numdev_minor);

    /*get device major number and minor number*/
    if (numdev_major) {
        //加载模块时是否传入非0的主设备号
        num_dev = MKDEV(numdev_major, numdev_minor);
        ret = register_chrdev_region(num_dev, DEVICE_MINOR_CNT, DEVICE_NAME);
        if (ret < 0) {
            printk(KERN_EMERG "register_chrdev_region req %d is failed!\n",
                   numdev_major);
        }
    } else {
        //动态获取设备号, DEVICE_NAME 在cat /proc/devices命令执行时显示
        ret = alloc_chrdev_region(&num_dev, numdev_minor, DEVICE_MINOR_CNT,
                                  DEVICE_NAME);
        if (ret < 0) {
            printk(KERN_EMERG "alloc_chrdev_region is failed!\n");
        }
        numdev_major = MAJOR(num_dev);
        printk(KERN_EMERG "adev_region req %d !\n", numdev_major);
    }

    /*set class*/
    myclass = class_create(THIS_MODULE, CLASS_NAME);

    /*kmalloc memory for device */
    led_devices =
        kmalloc(DEVICE_MINOR_CNT * sizeof(struct reg_dev), GFP_KERNEL);
    if (!led_devices) {
        ret = -ENOMEM;
        goto fail;
    }
    memset(led_devices, 0, DEVICE_MINOR_CNT * sizeof(struct reg_dev));

    /*注册两个led设备*/
    for (i = 0; i < DEVICE_MINOR_CNT; i++) {
        led_devices[i].data = kmalloc(REGDEV_SIZE, GFP_KERNEL);
        memset(led_devices[i].data, 0, REGDEV_SIZE);

        /*设备注册到系统*/
        cdev_init(&(led_devices[i].cdev), &led_fops);
        led_devices[i].cdev.owner = THIS_MODULE;
        ret = cdev_add(&(led_devices[i].cdev),
                       MKDEV(numdev_major, numdev_minor + i), 1);
        if (ret) {
            printk(KERN_EMERG "cdev_add %d is fail! %d\n", i, ret);
        } else {
            printk(KERN_EMERG "cdev_add %d is success!\n", numdev_minor + i);
        }

        /*创建设备节点*/
        device_create(myclass, NULL, MKDEV(numdev_major, numdev_minor + i),
                      NULL, DEVICE_NAME "%d", i);
    }

    /*set gpio*/
    ret = gpio_init();
    if (ret) {
        printk(KERN_EMERG "gpio_init failed!\n");
    }

    printk(KERN_EMERG "scdev_init!\n");
    return 0;

fail:
    /*注销设备号*/
    unregister_chrdev_region(MKDEV(numdev_major, numdev_minor),
                             DEVICE_MINOR_CNT);
    printk(KERN_EMERG "kmalloc is fail!\n");

    return ret;
}

static void scdev_exit(void) {
    int i;
    printk(KERN_EMERG "scdev_exit!\n");

    /*除去字符设备*/
    for (i = 0; i < DEVICE_MINOR_CNT; i++) {
        cdev_del(&(led_devices[i].cdev));
        /*摧毁设备节点函数d*/
        device_destroy(myclass, MKDEV(numdev_major, numdev_minor + i));
    }
    /*释放设备class*/
    class_destroy(myclass);
    /*释放内存*/
    kfree(led_devices);

    /*释放GPIO*/
    for (i = 0; i < LED_CNT; i++) {
        gpio_free(led_gpios[i]);
    }

    unregister_chrdev_region(MKDEV(numdev_major, numdev_minor),
                             DEVICE_MINOR_CNT);
}

module_init(scdev_init);
/*初始化函数*/
module_exit(scdev_exit);
/*卸载函数*/