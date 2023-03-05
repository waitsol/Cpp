#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

int main(int args, char *argv[])
{
    pid_t pid = fork();
    if (pid != 0)
    {
        // 退出父进程
        exit(0);
    }
    // 创建新会话
    setsid();
    // 退出当前会话的首进程，孙子进程成为会话唯一进程
    pid = fork();
    if (pid != 0)
    {
        // 退出父进程
        exit(0);
    }
    umask(0);
    int fd = open("demon.out", O_WRONLY | O_CREAT, 0644);
    if (fd < 0)
    {
        perror("open");
        return 1;
    }
    int ret = dup2(fd, STDOUT_FILENO);
    if (ret == -1)
    {
        printf("输出重定向失败 %s\n", strerror(errno));
    }

    //  dup2(fd, STDERR_FILENO);
    char *a[20] = {NULL};
    for (int i = 1; i < args; i++)
    {
        a[i - 1] = argv[i];
    }

    execvp(argv[1], a);
    return 0;
}
