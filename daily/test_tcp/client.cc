#include "tcpHead.h"
using namespace std;
int main()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in ser_addr;
    memset(&ser_addr, 0, sizeof(ser_addr));
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_port = htons(777);                   //转化端口号
    ser_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //回环地址
    int old_flags = fcntl(sockfd, F_GETFL);
    fcntl(sockfd, F_SETFL, old_flags | O_NONBLOCK);
    connect(sockfd, (struct sockaddr *)&ser_addr, sizeof(ser_addr));
    //连接服务器，需要知道服务器绑定的IP和端口
 
    char buf[BUFSIZ]={0};
    for (int i=0;i<BUFSIZ;i++)
    {
        buf[i]='a';
    }
    sleep(1);
    int n = write(sockfd, buf, BUFSIZ);
   // cout<<n<<endl;
    // n=  read(sockfd, buf, BUFSIZ);
    // cout << n << endl;
    // cout<<buf<<endl;
    close(sockfd);
  
    return 0;
}