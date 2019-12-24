// this file try to add spin lock to a memery using driver

#include <linux/cdev.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/types.h>

// spin lock related
#include <inux/spinlock.h>
#include <inux/spinlock_api_up.h>
#include <inux/spinlock_type_up.h>
#include <inux/spinlock_types.h>
#include <inux/spinlock_up.h>

#define GLOBAL_MEM_SIZE 0x1000
#define MEM_CLEAR 0x1
#define GLOBAL_MEM_MAJOR 0
#define GLOBAL_MEM_MINOR 0
static int globalmem_major = GLOBAL_MEM_MAJOR;
static int globalmem_minor = GLOBAL_MEM_MINOR;

struct globalmem_dev {
    struct cdev cdev;
    unsigned char mem[GLOBAL_MEM_SIZE];
};
struct globalmem_dev dev;

int global_mem_init(void);
void global_mem_exit(void);

static void globalmem_setup_cdev(void) {
    int err;
    int devno = MKDEV(globalmem_major, globalmem_minor);
    cdevinit(&dev.cdev, &globalmem_fops);
    dev.cdev.
}

int global_mem_init(void) {
    int ret = 0, i;
    dev_t num_dev;

    printk(KERN_EMERG "numdev_major is %d!\n", numdev_major);
    printk(KERN_EMERG "numdev_minor is %d!\n", numdev_minor);

    if (numdev_major) {
        num_dev = MKDEV(numdev_major, numdev_minor);
        ret = register_chrdev_region(num_dev, DEVICE_MINOR_NUM, DEVICE_NAME);
    } else {
        /*动态注册设备号*/
        ret = alloc_chrdev_region(&num_dev, numdev_minor, DEVICE_MINOR_NUM,
                                  DEVICE_NAME);
        /*获得主设备号*/
        numdev_major = MAJOR(num_dev);
        printk(KERN_EMERG "adev_region req %d !\n", numdev_major);
    }

    if (ret < 0) {
        printk(KERN_EMERG "register_chrdev_region req %d is failed!\n",
               numdev_major);
    }

    return 0;
}

module_init(global_mem_init);
module_exit(global_mem_exit);