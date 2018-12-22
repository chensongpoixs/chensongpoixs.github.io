#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	//文件描述符
	int fd;
	int len; //文件大小
	int ret; //状态码
	//打开文件
	fd = open("./text.log", O_RDWR);
	if (fd < 0)
	{
		perror("open error");
		return -1;
	}
	//文件大小
	len = lseek(fd, 0, SEEK_END);
	//映射到内核中
	void * ptr = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	if (ptr == MAP_FAILED)
	{
		perror("mmap error");
		return -1;
	}
	char * p = (char *) ptr;
	strcpy(p, "陈丽， 王蓉");


	//关闭
	munmap(ptr, len);
	close(fd);


	return 0;
}
