#ifndef INC_H
#define INC_H 1
//内连接多重引用不会参数 重定义
static int inc_static_a = 1;//内链接
static int inc_static_b = 1;
//int b=1;外连接
const int cont_a=2;//内链接


class inc
{
private:
    /* data */
public:
    inc(/* args */);
    ~inc();

    //类函数默认是内联函数  所以不会产生符号链接 导致重定义
    void fun1()
    {

    }
    void fun2();
    void fun3();
    void fun4();
};
//内联函数 不会产生符号链接 所以不对重定义
inline void inc::fun2()
{

}
// 外连接 会产生重定义
// void inc::fun3()
// {

// }



#endif