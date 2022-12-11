#include "tcpHead.h"
#include "thread"
#include "iostream"
#include "string.h"
using namespace std;

void ReadFd(int fd)
{
    char buf[BUFSIZ]={0};
    while(1)
    {
        int n=read(fd,buf,BUFSIZ);
        if(n<=0)
        {
            printf("exit %s\n",strerror(errno));
           // close(fd);
           sleep(5);;//直接close会合并 中间2次挥手
            close(fd);
             cout<<"close\n";
           return;
        }
        cout<<"recv "<<buf<<endl;
        memset(buf,0,BUFSIZ);
    }
}

int main()
{
    int fd = GetSockBlockFd("0.0.0.0",777);
    cout<<fd<<endl;
    struct sockaddr_in client_addr; // connector's address information
    socklen_t sin_size = sizeof(client_addr);

    size_t sz=1413;
    char *psend=new char[sz];
    memset(psend,0,sz);

   
    int new_fd = accept(fd, (struct sockaddr *)&client_addr, &sin_size);
    cout << "new connect " << new_fd << endl;
    thread t(std::bind(&ReadFd, new_fd));
    char buf[BUFSIZ] = {0};
    while (cin >> buf)
    {
        if(strstr(buf,"q")!=nullptr)
        {
            write(new_fd, psend,sz);
        }else{
            write(new_fd, buf, strlen(buf));
        }
    }
    

    return 0;
}