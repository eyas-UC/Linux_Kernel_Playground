#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

// user space code 


int main()
{
	int ret=0;
	int dev = open("/dev/mydevice",O_RDONLY);
	if(ret ==-1)
	{
		printf("unable to read the device something went wrong!\n");
		return -1;
	}
	printf("device was opened successfully!\n check your dmesg to see the callback registered in file_ops struct");
	close(dev);
	return 0;
}
