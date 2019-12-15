#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>

/*设备为杂项设备，驱动使用 platform_driver_register 注册，设备使用 platform_device_register 注册，设备节点由 misc_register 生成*/

/*驱动注册的头文件，包含驱动的结构体和注册和卸载的函数*/
#include <linux/platform_device.h>
/*注册杂项设备头文件*/
#include <linux/miscdevice.h>
/*注册设备节点的文件结构体*/
#include <linux/fs.h>

#define DRIVER_NAME "DeviceNodeTestModule"
#define DEVICE_NAME "DeviceNodeTestModule"
#define MISC_DEVICE_NAME "MISC_NAME_EXAMPLE"
MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Holden");

//device definition
//device definition
//device definition
void DeviceNodeTest_Device_release(struct device *dev)
{
	printk(KERN_EMERG "ENTER: DeviceNodeTest_Device_release\n");
	return;
}

struct platform_device DeviceNodeTest_Device = {
    .name   = DEVICE_NAME,
    .id             = -1,
    .dev = 
    {
        .release = DeviceNodeTest_Device_release,
    }
};

static int DeviceNodeTest_Device_init(void)
{
    printk(KERN_EMERG "ENTER: DeviceNodeTest_Device_init\n");
    printk(KERN_EMERG "platform_device_register try to registetr DeviceNodeTest_Device\n");
	int DeviceRegReturn = platform_device_register(&DeviceNodeTest_Device);
	printk(KERN_EMERG "platform_device_register return is %d\n",DeviceRegReturn);
	return DeviceRegReturn;
}

static void DeviceNodeTest_Device_exit(void)
{
    printk(KERN_EMERG "ENTER: DeviceNodeTest_Device_exit\n");		
    printk(KERN_EMERG "platform_device_unregister try to unregister DeviceNodeTest_Device\n");
	platform_device_unregister(&DeviceNodeTest_Device);
	return;
}

//misc device defenition
//misc device defenition
//misc device defenition
static long hello_ioctl(struct file *files, unsigned int cmd, unsigned long arg){
    printk("cmd is %d,arg is %d\n", cmd, arg);
    return 0;
}

static int hello_release(struct inode *inode, struct file *file){
    printk(KERN_EMERG "hello release\n");
    return 0;
}

static int hello_open(struct inode *inode, struct file *file){
    printk(KERN_EMERG "hello open\n");
    return 0;
}

static struct file_operations hello_ops = {
    .owner = THIS_MODULE,
    .open = hello_open,
    .release = hello_release,
    .unlocked_ioctl = hello_ioctl,
};

static  struct miscdevice DeviceNodeTest_MiscDevice = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = MISC_DEVICE_NAME,
    .fops = &hello_ops,
};

//driver definition
//driver definition
//driver definition
static int DeviceNodeTest_Driver_probe(struct platform_device *pdv){
    printk(KERN_EMERG "ENTER: DeviceNodeTest_Driver_probe\n");
    printk(KERN_EMERG "misc_register try to register DeviceNodeTest_MiscDevice\n");
    misc_register(&DeviceNodeTest_MiscDevice);
    return 0;
}

static int DeviceNodeTest_Driver_remove(struct platform_device *pdv){
    printk(KERN_EMERG "ENTER: DeviceNodeTest_Driver_remove\n");
    printk(KERN_EMERG "misc_deregister try to unregister DeviceNodeTest_MiscDevice\n");
    misc_deregister(&DeviceNodeTest_MiscDevice);
    return 0;
}

static void DeviceNodeTest_Driver_shutdown(struct platform_device *pdv){
    printk(KERN_EMERG "\tDeviceNodeTest_Driver_shutdown\n");
}

static int DeviceNodeTest_Driver_suspend(struct platform_device *pdv,pm_message_t pmt){
    printk(KERN_EMERG "\tDeviceNodeTest_Driver_suspend\n");
    return 0;
}

static int DeviceNodeTest_Driver_resume(struct platform_device *pdv){
    printk(KERN_EMERG "\tDeviceNodeTest_Driver_resume\n");
    return 0;
}

struct platform_driver DeviceNodeTest_Driver = {
    .probe = DeviceNodeTest_Driver_probe,
    .remove = DeviceNodeTest_Driver_remove,
    .shutdown = DeviceNodeTest_Driver_shutdown,
    .suspend = DeviceNodeTest_Driver_suspend,
    .resume = DeviceNodeTest_Driver_resume,
    .driver = {
        .name = DRIVER_NAME,
        .owner = THIS_MODULE,
    }
};

static int DeviceNodeTest_init(void)
{
    //creat device
    DeviceNodeTest_Device_init();

    //creat driver
    printk(KERN_EMERG "ENTER: platform_driver_register try to register DeviceNodeTest_Driver\n");
    int DriverState = platform_driver_register(&DeviceNodeTest_Driver);
    printk(KERN_EMERG "platform_driver_register return is %d\n",DriverState);

    return 0;
}

static void DeviceNodeTest_exit(void)
{
    //remove driver
    printk(KERN_EMERG "ENTER: DeviceNodeTest_exit\n");
    printk(KERN_EMERG "platform_driver_unregister try to unregister DeviceNodeTest_Driver!\n");
    platform_driver_unregister(&DeviceNodeTest_Driver);

    //remove device
    DeviceNodeTest_Device_exit();
}

module_init(DeviceNodeTest_init);
module_exit(DeviceNodeTest_exit);
