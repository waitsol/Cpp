#include "iostream"
#include "stdio.h"
#include "inc.h"
#include "done.h"
using namespace std;
extern int g_a; //done.cpp 定义一个符号编译没问题 ，链接的时候去别的编译单元找
int main()
{
    inc c;
    //c.fun4();

    
    done d;
    
    //因为include done.h 所以和done.h共享同一个编译单元 也就同享a
    d.done1();
    printf("main inc_static_a = %d addr=%p\n", ++inc_static_a, &inc_static_a);

    //因为是不同的编译单元 所以b地址不同 也就是2个变量  地址不同 
    d.done2();
    printf("main inc_static_b = %d addr=%p\n", ++inc_static_b, &inc_static_b);
    cout<<"g_a="<<g_a<<endl;
    return 0;
}