#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>

/**
读取数据mmap_read

**/

int main(int argc, char *argv[])
{
	//文件指针
	int fd;
	int len; //文件大小
	void * ptr; //映射内存地址
	char buf[1024];
	//打开文件
	fd = open("./text.log", O_RDWR);
	if (fd < 0)
	{
		perror("open error");
		return -1;
	}

	//文件大小
	len = lseek(fd, 0, SEEK_END);
	//创建映射
	ptr = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (ptr == MAP_FAILED)
	{
		perror("mmap error");
		return -1;
	}

	//=============读取 map内存的数据===================
	//while (1)
	//{
		//memset(buf, 0x00, sizeof(buf));
	//	read(fd, buf, sizeof(buf));
	char *p = (char *) ptr;
		printf("读取数据：%s\n", p);
	//}
	munmap(ptr, len);

	//关闭文件操作
	close(fd);
	
	return 0;
}
