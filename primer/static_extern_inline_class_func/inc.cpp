#include "inc.h"
inc::inc(/* args */)
{
}

inc::~inc()
{
}
//error 如果在类体外定义inline函数，则心须将类定义和成员函数的定义都放在同一个头文件中，否则编译时无法进行置换
//只有在类外定义的成员函数规模很小而调用频率高时，才将此成员函数指定为内置函数。
//但是发现如果将函数声明和定义分开写在.h和.cc里面，虽然能编译成模块，但是却无法链接。
//https://www.cnblogs.com/justinyo/archive/2013/03/13/2957504.html
// inline void inc::fun4()
// {

// }