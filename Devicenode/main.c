#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define MISC_PATH "/dev/MISC_NAME_EXAMPLE"

main(){
	int fd;
	char *misc_node = MISC_PATH;
	
/*O_RDWR只读打开,O_NDELAY非阻塞方式*/	
	if((fd = open(misc_node, O_RDWR|O_NDELAY))<0){
		printf("APP open %s failed", misc_node);
	}
	else{
		printf("APP open %s success", misc_node);
		ioctl(fd,1,6);
	}
	
	close(fd);
}