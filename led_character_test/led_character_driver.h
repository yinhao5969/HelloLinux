#ifndef _LED_CHARACTER_DRIVER_H_
#define _LED_CHARACTER_DRIVER_H_

#ifndef DEVICE_NAME
#define DEVICE_NAME "led_character_device"
#endif

#ifndef CLASS_NAME
#define CLASS_NAME "led_character_class"
#endif

#ifndef DEVICE_MINOR_CNT
#define DEVICE_MINOR_CNT 2
#endif

#ifndef DEV_MAJOR
#define DEV_MAJOR 0
#endif

#ifndef DEV_MINOR
#define DEV_MINOR 44
#endif

#ifndef REGDEV_SIZE
#define REGDEV_SIZE 3000
#endif

struct reg_dev {
    char *data;
    unsigned long size;
    struct cdev cdev;
};
#endif