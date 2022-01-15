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

        connect(serv_sock,(struct sockaddr *)&serv_addr, sizeof(serv_addr));
        long long k=0;
        long long ans=1e12;
        
        while(ans-k>=0)
        {
                char buf[100];
                sprintf(buf,"-%lld",k);
                int n=write(serv_sock,buf,strlen(buf));
              //  cout<<k++<< "  "<<n<<endl;
              
        }
        cout<<1e12-ans<<endl;
        return 0;
}
