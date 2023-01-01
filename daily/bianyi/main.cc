#include "net.h"
#include "role.h"
struct Test
{
    /* data */
    Net n;
    Role *r;

};

int main()
{

    // 分别创建一个对象  net对象发生改变  main.cc.o需要重新编译 role改变不需要
    //查看编译时间命令    find . -name "*.o"|xargs ls -l 
    Net *pnet=GetNet();
    Role *prole=GetRole();
    

    return 0;
}