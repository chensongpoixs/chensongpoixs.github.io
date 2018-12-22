#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/mman.h>



int main(int argc, char *argv[])
{
	//文件描述符
	int fd;
	int len;
	fd = open("./text.log", O_RDWR);
	if (fd < 0)
	{
		perror("open error");
		return -1;
	}
	//文件大小
	len = lseek(fd, 0, SEEK_END);

	//参数一：NULL
	//参数二：文件的大小，不能为0
	//参数三：权限
	//参数四：是否同步的硬盘中
	//参数五：文件指针
	//参数六：文件开始位置
	void *ptr = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);


	char *p = (char*)ptr;
	strcpy(p, "陈丽");

	//参数一：文件指针
	//参数二：文件大小
	munmap(ptr, len);
	//关闭文件
	close(fd);


	return 0;
}
