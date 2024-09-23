#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
	int fd;
	char * address;
	struct stat len_struct;


	fd = shm_open("shared_memory_local",O_RDONLY, 0);

	if (fd == -1)
	{
		printf("shm error\n");
		perror("shm open error\n");
		exit(-1);
	}
	printf("before fstat\n");
	fstat(fd,&len_struct);
	printf("fstat worked!\n");
	address = (char *) mmap(NULL, len_struct.st_size, PROT_READ, MAP_SHARED, fd, 0);

	if (address == MAP_FAILED)
	{
		printf("mmap failed\n");
		perror("mmap failed\n");
		exit(-1);
	}

	printf("\n read data from shared memory:\n(%s)\n",address);
	exit(0);

}
