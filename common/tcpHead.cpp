#include"tcpHead.h"
int GetSockFd(std::string addr , int port)
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
    ERR(ret == 0);
    //绑定文件描述符和服务器的ip和端口号
    ret = bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    ERR(ret == 0);
    //进入监听状态，等待用户发起请求
    ret = listen(serv_sock, 20);
    ERR(ret == 0);

    old_flags = fcntl(serv_sock, F_GETFL);
    ERR(old_flags != -1);
    fcntl(serv_sock, F_SETFL, old_flags | O_NONBLOCK);
    return serv_sock;
EXIT:
    return -1;
}
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
    ERR(ret == 0);
    //绑定文件描述符和服务器的ip和端口号
    ret = bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    ERR(ret == 0);
    //进入监听状态，等待用户发起请求
    ret = listen(serv_sock, 20);
    ERR(ret == 0);
    return serv_sock;
EXIT:
    return -1;
}