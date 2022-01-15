#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <vector>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <sys/epoll.h>
#include <unordered_set>
#include <thread>
#include <functional>
#include <fcntl.h>
using namespace std;
#define ERR(condition)                        \
    do                                        \
    {                                         \
        if (!(condition))                     \
        {                                     \
            printf("error %s\n", #condition); \
            goto Exit;                        \
        }                                     \
    } while (0);

class Epoll
{
    typedef void(Epoll::*eventFunc)(void *);
    struct Event_Msg
    {
        int fd;
        eventFunc func;
        void *argv;
        char buf[1024];
    };

public:
    Epoll()
    {
        m_epfd = -1;
        m_eventSize = 100;
        m_vecEvent.reserve(m_eventSize);
    }
    void loop()
    {
        cout<<"wait..."<<endl;
        int ret = epoll_wait(m_epfd, m_vecEvent.data(), m_eventSize, -1);
        ERR(ret != -1);
        for (int i = 0; i < ret; i++)
        {
            thread t(std::bind(&Epoll::handleMessage, this, (void*)m_vecEvent[i].data.ptr));
            t.detach();
            //handleMessage(m_vecEvent[i].data.ptr);
        }
        return;
    Exit:
        return;
    }

    bool init()
    {
        m_epfd = epoll_create(1);
        Event_Msg *msg = new Event_Msg;
        int listenfd = -1;
        ERR(m_epfd != -1);

        listenfd = GetSockFd();
        ERR(listenfd != -1);
        epoll_event event;
        event.events = EPOLLIN | EPOLLET;
     
        msg->fd=listenfd;
        msg->func =&Epoll::Accept;
        event.data.ptr=msg;

        cout << "listenfd=" << listenfd << endl;
        epoll_ctl(m_epfd, EPOLL_CTL_ADD, listenfd, &event);
        return true;
        Exit:
            delete msg;
            return false;
    }

private:
    int GetSockFd(string addr = "127.0.0.1", int port = 777)
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
    Exit:
        return -1;
    }
    void handleMessage(void *argv)
    {
        Event_Msg *msg=(Event_Msg*)argv;
        (this->*(msg->func))(argv);
    }
    void Accept(void *argv)
    {
        int fd  = ((Event_Msg *)argv)->fd;
        Event_Msg *msg=new Event_Msg;
        int confd=accept(fd,nullptr,nullptr);
        ERR(confd!=-1)
        epoll_event event;
        event.events = EPOLLIN | EPOLLET|EPOLLOUT;
        msg->fd = confd;
        msg->func = &Epoll::Read;
        event.data.ptr = msg;
        //cout << "confd=" << confd << endl;
        epoll_ctl(m_epfd, EPOLL_CTL_ADD, confd, &event);
        Exit:
            delete msg;
            return;
    }
    void Read(void *argv)
    {
        int fd = ((Event_Msg *)argv)->fd;

        char buf[1024]={0};
        int len=0;
        len=read(fd,buf,sizeof(buf));
        ERR(len >0);
        buf[len]=0;
        cout<<buf;
        write(fd,buf,len);
        return;
    Exit:
        delete (Event_Msg*)argv;
        epoll_ctl(m_epfd, EPOLL_CTL_DEL, fd,nullptr);
        printf("epfd del fd =%d\n",fd);
        return;
    }

private:
    int m_epfd;
    vector<epoll_event> m_vecEvent;
    int m_eventSize;
};

int main()
{
    Epoll e;
    e.init();
    while (1)
    {
        e.loop();
    }

    return 0;
}