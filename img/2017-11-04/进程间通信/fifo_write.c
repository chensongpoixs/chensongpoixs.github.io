/*************************************************************************
	> File Name: fifo_write.c
	> Author: songli
	> QQ: 2734030745
	> Mail: 15850774503@163.com
    > CSDN: http://my.csdn.net/Poisx
	> github: https://github.com/chensongpoixs
	> Created Time: Fri 20 Oct 2017 11:22:50 PM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>



int main(int argc, char *argv[])
{
    // 文件描述符
    int fd;
    
    int ret;
    //判断是否创建管道文件
    if ((ret = access("./myfifo", F_OK)) == -1)
    {
        //创建管道
        ret =  mkfifo("./myfifo", 0777);
        if (ret < 0)
        {
            perror("mkfifo error");
            return -1;
        }
    }

    //打开文件
    fd = open("./myfifo", O_RDWR);
    if (fd < 0)
    {
        perror("open error");
        return -1;
    }

    //=====================写数据到管道中============================

    while (1)
    {
        sleep(1);
        write(fd, "陈丽, 杨艳", strlen("杨艳"));
    }

    close(fd);


	return 0;
}
