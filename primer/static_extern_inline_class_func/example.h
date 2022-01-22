int a;                //a有外部链接，不能在头文件中定义。
extern int b = 10;    //同上。
const int c = 2;      //c具有内部链接，可以定在头文件中但应该避免。
static int d = 3;     //同上。
static void func() {} //同上。
void func2() {}       //同a。
void func3();         //可以。仅仅是声明。并不会导致符号名被写入目标文件。
class A
{
public:
    static int e; //可以，具有内部链接。
    int f;        //可以，同上。
    void
    func4(); //声明，内部链接。同上。
};
int A::e = 10;       //不可以在头文件中包含具有外部链接的定义。符号名别写入目标文件。
void A :: func4() //不可以，类成员函数。外部连接。
{
    //,......
}