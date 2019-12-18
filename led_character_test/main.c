#include <stdio.h>

#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// 0 0 led2关
// 1 0 led2开
// 0 1 led3关
// 1 1 led3开
/*argv[1] is cmd , argv[2] is io_arg*/
int main(int argc, char **argv) {
    int fd;
    char *lednode = "/dev/led_character_device0";

    /*O_RDWR只读打开,O_NDELAY非阻塞方式*/
    if ((fd = open(lednode, O_RDWR | O_NDELAY)) < 0) {
        printf("APP open %s failed!\n", lednode);
    } else {
        printf("APP open %s success!\n", lednode);
        ioctl(fd, atoi(argv[1]), atoi(argv[2]));
        printf("APP ioctl %s ,cmd is %s! io_arg is %s!\n", lednode, argv[1],
               argv[2]);
    }

    close(fd);
}