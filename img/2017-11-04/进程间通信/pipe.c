/*************************************************************************
	> File Name: pipe.c
	> Author: songli
	> QQ: 2734030745
	> Mail: 15850774503@163.com
    > CSDN: http://my.csdn.net/Poisx
	> github: https://github.com/chensongpoixs
	> Created Time: Fri 20 Oct 2017 10:22:29 PM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>



int main(int argc, char *argv[])
{
    //管道数组文件描述符
    int fd[2];       
    int ret;
    

    int i, n = 2;
    //创建管道
    ret = pipe(fd);

    //文件描述符
    pid_t pid;

    if (ret < 0)
    {
        perror("pipe error");
        return -1;
    }

    for (i = 0; i < n; i++)
    {
        //创建子进程
        pid = fork();
        if (pid < 0) // 异常处理
        {
            perror("fork error");
            return -1;
        }
        else if (pid > 0) // 父进程的操作
        {
            printf("father fpid:%d, cpid:%d\n", getpid(), pid);
        }
        else if (pid == 0) //子线程的操作
        {
            printf("child fpid:%d, cpid:%d\n", getppid(), getpid());
            break; //防止子线程创建子线程
        }
    }
    //===============  子线程的操作=============================
    if (i == 0)
    {
        printf("no child n = %d, cpid:%d\n", i, getpid());
        //=================写入数据 =====================
        close(fd[0]); //关闭读取文件描述符
        //标准输入也是 文件描述符   中的1
        dup2(fd[1], STDOUT_FILENO);
        // 执行  写入命令
        execlp("ps", "ps", "-ef", NULL);
        close(fd[1]); //关闭写入文件描述符
        return 0; //signal  正常退出
    }
    if (i == 1)
    {
        printf("no child n = %d, cpid:%d\n", i, getpid());
        //=================写入数据 =====================
        close(fd[1]); //关闭读取文件描述符
        //标准输出也是 文件描述符   中的1
        dup2(fd[0], STDIN_FILENO);
        // 执行  写入命令
        execlp("grep", "grep", "--color", "bash", NULL);
        close(fd[0]); //关闭写入文件描述符
        return 0; //signal  正常退出
    }

    //=================      主线程的操作 ========================
    if (i == n)
    {
        //pcb 进程块pid
        pid_t wpid;
        int status;

        while (1)
        {
            //等待线程
            wpid = waitpid(-1, &status, WNOHANG );
            if (wpid == 0) //有子线程的操作
            {
                sleep(1);
                continue;
            }
            else if (wpid == -1) // 没有子线程的操作
            {
                printf("退出子线程的wpid：%d\n", wpid);
                exit(0);
            }
            else if (wpid > 0)  //什么原因退出
            {
                if (WIFEXITED(status)) //正常退出
                {
                    printf("子线程wpid:%d, status:%d\n", wpid, WEXITSTATUS(status));
                }
                if (WIFSIGNALED(status))
                {
                    printf("子线程wpid:%d, status:%d\n", wpid, WTERMSIG(status));
                    
                }
            }
        }

    }
	return 0;
}
