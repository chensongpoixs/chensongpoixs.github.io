/*************************************************************************
	> File Name: pipe_fcntl.c
	> Author: songli
	> QQ: 2734030745
	> Mail: 15850774503@163.com
    > CSDN: http://my.csdn.net/Poisx
	> github: https://github.com/chensongpoixs
	> Created Time: Fri 20 Oct 2017 10:47:08 PM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>



int main(int argc, char *argv[])
{
    //数组文件描述符
    int fd[2];
    int ret;
    char buf[1024];

    //创建管道
    ret = pipe(fd);
    if (ret < 0)
    {
        perror("pipe error");
        return -1;

    }

    //pcb进程控制块pid
    pid_t pid;

    //创建子线程
    pid = fork();
    if (pid < 0) //异常处理
    {
        perror("fork error");
        return -1;
    }
    else if (pid > 0) //父进程的处理
    {
        printf("father fpid:%d, cpid:%d\n", getpid(), pid);

        //==============读取数据==================
        close(fd[1]); //关闭写文件操作
        //==================    在同一个进程中使用非阻塞使用fcntl函数 =========================================
        int flags = fcntl(fd[0], F_GETFL );
        flags |= O_NONBLOCK;
        fcntl(fd[0], F_SETFL, flags);
        //=============================================================
        while(1)
        {
            sleep(1);
            memset(buf, 0x00, sizeof(buf));
            ret = read(fd[0], buf, sizeof(buf));
            printf("读取数据的大小：%d buf = %s\n", ret, buf);
        }
        close(fd[0]);

    }
    else if (pid == 0) //子进程的操作
    {
       printf("child fpid:%d, cpid:%d\n", getppid(), getpid());
       //========== 写操作=================== 
        close(fd[0]); //关闭读取文件描述符
        int i = 0;
        while (1)
        {
            sleep(1);
            memset(buf, 0x00, sizeof(buf));
            sprintf(buf, "[%d]:[%s]", i++, "陈丽， 杨艳");
            write(fd[1], buf, strlen(buf));
        }
        close(fd[1]);
    }




	return 0;
}
