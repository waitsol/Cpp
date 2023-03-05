
#include <iostream>
#include <cassert>
using namespace std;
class test
{
public:
    test(int a)
    {
        b = a;
        cout << "构造函数" << endl;
    }
    test(const test &a)
    {
        cout << "复制构造" << endl;
    }
    ~test()
    {
        cout << "析构" << endl;
    }
    int b;
    inline const static int siatic_a = 2;
};
void f(const int *a)
{
    cout << *a << endl;
}
int main()
{

    cout << test::siatic_a << endl;
    f(&test::siatic_a);
}