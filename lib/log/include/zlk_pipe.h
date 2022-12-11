#ifndef __ZLK_PIPE_H
#define __ZLK_PIPE_H
#include <mutex>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include "iostream"
#include "tcpHead.h"
using namespace std;
class zlk_pipe
{
public:
    zlk_pipe()=default;
    void init()
    {
        m_fd = socket(AF_INET, SOCK_STREAM, 0);
        struct sockaddr_in ser_addr;
        memset(&ser_addr, 0, sizeof(ser_addr));
        ser_addr.sin_family = AF_INET;  
        ser_addr.sin_port = htons(7777);                    //转化端口号
        ser_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //回环地址
 
        //cout << "connect ret = "<< " mfd = " << m_fd << endl;
        int ret = connect(m_fd, (struct sockaddr *)&ser_addr, sizeof(ser_addr));
     //   cout<<"connect ret = "<<ret<<" mfd = "<<m_fd<<endl;
        if (ret ==-1)
        {
            cout << strerror(errno) << endl;
        }
    }
    void notify(){
        write(m_fd, "a", 1);
    }
    int  getFd(){
        return m_fd;
    }
    void close()
    {
        ::close(m_fd);
    }
private:
    int m_fd;
};

#endif