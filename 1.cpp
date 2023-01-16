#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string.h>
#include <iostream>
#include <sys/mman.h>

#include <unordered_set>
#include <thread>
#include <functional>
#include <fcntl.h>
#ifndef WIN32
#include <sys/epoll.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif // !WIN32
#include <fstream>
using namespace std;
int GetSockBlockFd(std::string addr, int port)
{
    int serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    //初始化socket元素
    int ret = 0;
    int old_flags = -1;
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(addr.c_str());
    serv_addr.sin_port = htons(port);
    int opt = 1;
    //端口复用
    ret = setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    //绑定文件描述符和服务器的ip和端口号
    ret = bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    //进入监听状态，等待用户发起请求
    ret = listen(serv_sock, 20);
    return serv_sock;

}

int main()
{

    void *p = NULL;
    int fd = -1;
    // 打开文件
    fd = open("temp", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (-1 == fd)
    {
        printf("文件打开失败...\n");
        return -1;
    }

    // 因为我们文件不能是一个0大小的文件，所以我们需要修改文件的大小
    // 有两种方式：leek，write，或者ftruncate都可以

    /*
    // 改变一个文件的读写指针
    lseek(fd, 1023, SEEK_END);
    // 写入一个结束符\0
    write(fd, "\0", 1);
    */
    // 我们还是用这种，比较方便，直接就修改了，和上面效果一样
    int size=1024*1024*100;
    ftruncate(fd, size);

    // 创建一个内存映射，让内和指定一个映射地址，大小为1024，可读可写，共享，映射到这个fd上
    p = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (p == MAP_FAILED)
    {
        printf("mmap failed\n");
        close(fd);
        return -1;
    }
      while (1);

    return 0;
}