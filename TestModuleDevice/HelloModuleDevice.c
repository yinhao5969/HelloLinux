#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>

#define DEVICE_NAME "HelloModule"

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("YINHAO");

void HelloModule_release(struct device *dev) {
    printk(KERN_EMERG "YINHAO ENTER HelloModule releaseTEST\n");
    return;
}

struct platform_device s3c_device_hello_ctl = {
    .name = DEVICE_NAME,
    .id = -1,
    .dev = {
        .release = HelloModule_release,
    }};

static int hello_device_init(void) {
    printk(KERN_EMERG "YINHAO ENTER DEVICE REGISTER TEST\n");
    int DeviceRegReturn = platform_device_register(&s3c_device_hello_ctl);
    printk(KERN_EMERG "\tDeviceRegReturn is %d\n", DeviceRegReturn);
    return DeviceRegReturn;
}

static void hello_device_exit(void) {
    printk(KERN_EMERG "YINHAO LEAVE DEVICE REGISTER TEST\n");
    platform_device_unregister(&s3c_device_hello_ctl);
    return;
}

module_init(hello_device_init);
module_exit(hello_device_exit);
