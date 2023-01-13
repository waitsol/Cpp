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
    int init()
    {
        m_fd=-1;
        int fd[2] = {0};
        if (pipe(fd))
        {
            return -1;
        }
        m_fd=fd[1];
        return fd[0];
    }
    void notify(){
        write(m_fd, "a", 1);
    }
    int  getFd(){
        return m_fd;
    }
    void close()
    {
        if(m_fd>=0)
        {
            ::close(m_fd);
        }
    }
private:
    int m_fd;
};

#endif