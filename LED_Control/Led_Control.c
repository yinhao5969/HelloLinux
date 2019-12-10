#include <linux/init.h>
#include <linux/module.h>

/*驱动注册的头文件，包含驱动的结构体和注册和卸载的函数*/
#include <linux/platform_device.h>
/*注册杂项设备头文件*/
#include <linux/miscdevice.h>
/*注册设备节点的文件结构体*/
#include <linux/fs.h>

/*Linux中申请GPIO的头文件*/
#include <linux/gpio.h>
/*三星平台的GPIO配置函数头文件*/
/*三星平台EXYNOS系列平台，GPIO配置参数宏定义头文件*/
#include <plat/gpio-cfg.h>
#include <mach/gpio.h>
/*三星平台4412平台，GPIO宏定义头文件*/
#include <mach/gpio-exynos4.h>

#define DRIVER_NAME "led_ctl"
#define DEVICE_NAME "led_ctl"
#define MISC_DEVICE_NAME "led_ctl_misc"


MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Holden");

//device definition
//device definition
//device definition
void LedCtl_Device_release(struct device *dev)
{
	printk(KERN_EMERG "ENTER: LedCtl_Device_release\n");
	return;
}

struct platform_device LedCtl_Device = {
    .name   = DEVICE_NAME,
    .id             = -1,
    .dev = 
    {
        .release = LedCtl_Device_release,
    }
};

static int LedCtl_Device_init(void)
{
    printk(KERN_EMERG "ENTER: LedCtl_Device_init\n");
    printk(KERN_EMERG "platform_device_register try to registetr LedCtl_Device\n");
	int DeviceRegReturn = platform_device_register(&LedCtl_Device);
	printk(KERN_EMERG "platform_device_register return is %d\n",DeviceRegReturn);
	return DeviceRegReturn;
}

static void LedCtl_Device_exit(void)
{
    printk(KERN_EMERG "ENTER: LedCtl_Device_exit\n");		
    printk(KERN_EMERG "platform_device_unregister try to unregister LedCtl_Device\n");
	platform_device_unregister(&LedCtl_Device);
	return;
}

//misc definition
static long LedCtl_ioctl( struct file *files, unsigned int cmd, unsigned long arg){
    printk("cmd is %d,arg is %d\n",cmd,arg);

    if(cmd > 1){
        printk(KERN_EMERG "cmd is 0 or 1\n");
    }
    if(arg > 1){
        printk(KERN_EMERG "arg is only 1\n");
    }

    gpio_set_value(EXYNOS4_GPL2(0),cmd);

    return 0;
}

static int LedCtl_release(struct inode *inode, struct file *file){
    printk(KERN_EMERG "led_ctl release\n");
    return 0;
}

static int LedCtl_open(struct inode *inode, struct file *file){
    printk(KERN_EMERG "led_ctl open\n");
    return 0;
}

static struct file_operations LedCtl_ops = {
    .owner = THIS_MODULE,
    .open = LedCtl_open,
    .release = LedCtl_release,
    .unlocked_ioctl = LedCtl_ioctl,
};

static  struct miscdevice LedCtl_misc_dev = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = MISC_DEVICE_NAME,
    .fops = &LedCtl_ops,
};

//driver
static int LedCtl_probe(struct platform_device *pdv){
    int ret;

    printk(KERN_EMERG "\tinitialized\n");

    ret = gpio_request(EXYNOS4_GPL2(0), "LEDS");
    if(ret < 0){
        printk(KERN_EMERG "gpio_request EXYNOS4_GPL2(0) failed!\n");
        return ret;
    }

    s3c_gpio_cfgpin(EXYNOS4_GPL2(0),S3C_GPIO_OUTPUT);

    gpio_set_value(EXYNOS4_GPL2(0),0);

    misc_register(&LedCtl_misc_dev);

    return 0;
}

static int LedCtl_remove(struct platform_device *pdv){

    printk(KERN_EMERG "\tremove\n");
    misc_deregister(&LedCtl_misc_dev);
    return 0;
}

static void LedCtl_shutdown(struct platform_device *pdv){
	
    ;
}

static int LedCtl_suspend(struct platform_device *pdv,pm_message_t pmt){
	
    return 0;
}

static int LedCtl_resume(struct platform_device *pdv){
	
    return 0;
}

struct platform_driver LedCtl_driver = {
    .probe = LedCtl_probe,
    .remove = LedCtl_remove,
    .shutdown = LedCtl_shutdown,
    .suspend = LedCtl_suspend,
    .resume = LedCtl_resume,
    .driver = {
        .name = DRIVER_NAME,
        .owner = THIS_MODULE,
    }
};

static int LedCtl_init(void)
{
    //creat device
    LedCtl_Device_init();
    
    //creat driver
    printk(KERN_EMERG "ENTER: platform_driver_register try to register LedCtl_driver\n");
    int DriverState = platform_driver_register(&LedCtl_driver);
    printk(KERN_EMERG "platform_driver_register return is %d\n",DriverState);
    return 0;
}

static void LedCtl_exit(void)
{
    //remove driver
    printk(KERN_EMERG "ENTER: LedCtl_exit\n");
    printk(KERN_EMERG "platform_driver_unregister try to unregister LedCtl_driver!\n");
    platform_driver_unregister(&LedCtl_driver);

    //remove device
    LedCtl_Device_exit();
}

module_init(LedCtl_init);
module_exit(LedCtl_exit);
