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




    struct timeval tv;              // 超时时间
	tv.tv_sec = 2;
    tv.tv_usec = 500;               // 注意单位是微秒
 
    fd_set rdfds;
    FD_ZERO(&rdfds);                // 描述集初始化
    FD_SET(STDIN_FILENO, &rdfds);   // STDIN_FILENO是标准输入， 塞入描述集
    FD_SET(serv_sock, &rdfds);
    int maxfd=serv_sock+1;
    bool m[1024]={false};
    m[serv_sock]=true;
    while(1)
    {
        for(int i=serv_sock;i<maxfd;i++)
            if(m[i])
                FD_SET(i,&rdfds);
        int iRet = select(maxfd, &rdfds, NULL, NULL, nullptr);  // 注意注意
        cout<<"select\n";
        if(iRet < 0)
        {
            printf("selcet error, iRet %d\n", iRet);
            return -1;
        }

        if(0 == iRet)
        {
            tv.tv_sec = 2;
            tv.tv_usec = 500;               // 注意单位是微秒
            printf("timeout \n");
        }else{
            for(int i=serv_sock+1;i<maxfd;i++)
            {
                if(FD_ISSET(i,&rdfds))
                {
                    char buf[1024]={0};
                    int n = read(i,buf,1024);
                    if(n<=0)
                    {
                        m[i]=false;
                        FD_CLR(i,&rdfds);
                        cout<<"fd = "<<i<<" close "<<endl;

                    }
                    else
                        cout<<"fd = "<<i<<" read "<<buf;

                }
                
            }
        }
        if (FD_ISSET(serv_sock, &rdfds)) 
        {
            struct sockaddr_in client_addr; // connector's address information
            socklen_t sin_size = sizeof(client_addr);
            int new_fd = accept(serv_sock, (struct sockaddr *)&client_addr, &sin_size);
            if (new_fd <= 0) {
                perror("accept");
                continue;
            }
            if(new_fd<=maxfd)
            {
                m[new_fd]=true;
                if(new_fd==maxfd)
                    maxfd++;
                printf("fd = %d connect\n",new_fd);
            }

        }
    }
	return 0;
}