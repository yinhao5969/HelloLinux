#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>

#define DRIVER_NAME "HelloModule"

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("YINHAO");

static int hello_probe(struct platform_device *pdv) {
    printk(KERN_EMERG "YINHAO ENTER HELLO PROBE\n");
    return 0;
}

static int hello_remove(struct platform_device *pdv) {
    printk(KERN_EMERG "YINHAO ENTER HELLO REMOVE\n");
    return 0;
}

static int hello_shutdown(struct platform_device *pdv) {
    printk(KERN_EMERG "YINHAO ENTER HELLO SHUTDOWN\n");
    return 0;
}

static int hello_resume(struct platform_device *pdv) {
    printk(KERN_EMERG "YINHO ENTER HELLO RESUME\n");
    return 0;
}

static int hello_suspend(struct platform_device *pdv) {
    printk(KERN_EMERG "YINHO ENTER HELLO SUSPEND\n");
    return 0;
}

struct platform_driver HelloModule = {.probe = hello_probe,
                                      .remove = hello_remove,
                                      .shutdown = hello_shutdown,
                                      .resume = hello_resume,
                                      .suspend = hello_suspend,
                                      .driver = {
                                          .name = DRIVER_NAME,
                                          .owner = THIS_MODULE,
                                      }};

static int hello_init(void) {
    printk(KERN_EMERG "YINHAO ENTER MODULE TEST\n");

    int DriverState;

    DriverState = platform_driver_register(&HelloModule);

    printk(KERN_EMERG "\tDriverState is %d\n", DriverState);

    return 0;
}

static void hello_exit(void) {

    printk(KERN_EMERG "YINHAO LEAVE MODULE TEST\n");
    platform_driver_unregister(&HelloModule);
    return;
}

module_init(hello_init);
module_exit(hello_exit);
