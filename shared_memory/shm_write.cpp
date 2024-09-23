#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>



int main(int argc, char *argv[])
{
	int fd;
	size_t len;
	char * address;
	

	/* open the shared memory file descriptor
	 * if read/write if existing otherwise create the file with access writes 660 */
	fd = shm_open("shared_memory_local", O_RDWR | O_CREAT, 0660);
	if (fd == -1)
	{
		printf("error opening shared memory!\n");
		perror("shm_open error\n");
		exit(-1);
	}

	printf("opened fd successfully!\n");
	/* define the length of the shared memory using ftruncate */
	/* technically truncating it to only our size of data */
	len = strlen(argv[1]);
	if (ftruncate(fd, len) == -1)
	{
		perror("ftruncate error\n");
		exit(-1);
	}

	printf("data size to be written is %i\n",(int) len);
	/* system call to mmap to map the allocated memory to the virtual shared memory */
	/* protection for read and write */
	address =(char*) mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (address == MAP_FAILED)
	{
		perror("mmap error\n");
		exit(-1);
	}

	if (close(fd) ==-1)
	{
		perror("close fd failed\n");
	}

	memcpy(address, argv[1],len);
	exit(0);
}
