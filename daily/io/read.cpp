#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/time.h>
#include<sys/types.h>
#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include<iostream>
#include<unordered_set>
using namespace std;
int main()
{
        int serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

        //初始化socket元素
        struct sockaddr_in serv_addr;
        memset(&serv_addr, 0, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        serv_addr.sin_port = htons(1234);

        setsockopt(serv_sock,SOL_SOCKET ,SO_REUSEADDR,nullptr,sizeof(1));
        //绑定文件描述符和服务器的ip和端口号
        bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

        //进入监听状态，等待用户发起请求
        listen(serv_sock, 20);
        struct sockaddr_in client_addr; // connector's address information
        socklen_t sin_size = sizeof(client_addr);
        int new_fd = accept(serv_sock, (struct sockaddr *)&client_addr, &sin_size);
        long long ans=0;
        while(1)
        {
                char buf[1024]={0};
                int n=read(new_fd,buf,1024);
                buf[n]=0;
              //  cout<<buf<<endl;
                ans+=atoi(buf);
                usleep(100);
        }

        cout<<ans<<endl;
        return 0;
}
