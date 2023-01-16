#include "tcpHead.h"
using namespace std;
struct  Msg
{
    int len;
    char *msg;
    /* data */
};

int main()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in ser_addr;
    memset(&ser_addr, 0, sizeof(ser_addr));
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_port = htons(2013);                   //转化端口号
    ser_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //回环地址
    int old_flags = fcntl(sockfd, F_GETFL);
    fcntl(sockfd, F_SETFL, old_flags | O_NONBLOCK);
    connect(sockfd, (struct sockaddr *)&ser_addr, sizeof(ser_addr));
    //连接服务器，需要知道服务器绑定的IP和端口
    Msg msg;
    msg.len=10;
    msg.msg = new char[msg.len];
    for (int i = 0; i < msg.len;i++)
        msg.msg[i]='a'+i;
    msg.msg[0] = 9;
    msg.msg[1] = 8;
    int n = write(sockfd, msg.msg,  msg.len);
    sleep(1000);
    close(sockfd);
  
    return 0;
}