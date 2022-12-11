#include "tcpHead.h"
using namespace std;
int main()
{
    int fd = GetSockBlockFd();
    cout<<fd<<endl;

    struct sockaddr_in client_addr; // connector's address information
    socklen_t sin_size = sizeof(client_addr);
    int new_fd = accept(fd, (struct sockaddr *)&client_addr, &sin_size);
    cout<<new_fd<<endl;
    string s = "send to client";
    write(new_fd, s.c_str(), s.length());
    sleep(1);
    char buf[BUFSIZ]={0};
    read(new_fd,buf,BUFSIZ);
    cout<<buf<<endl;
    // string s = "send to client";
    // write(new_fd, s.c_str(), s.length());
    // int n = read(new_fd, buf, BUFSIZ);
    // cout<<n<<endl;
    // if (n==0)
    // {
    //     s="sever close";
    //     n = write(new_fd, s.c_str(), s.length());
    //     cout << n << endl;
    //     close(new_fd);

    //     close(fd);
    // }
    return 0;   
}