#include "iostream"
using namespace std;

class testDemo
{
public:
    testDemo(int _num)
    {
        std::cout << "调用构造函数" << endl;
        p = new int(_num);
    }
    testDemo(const testDemo &other)
    {
        std::cout << "调用拷贝构造函数" << endl;
        p = new int(*other.p);
    }
    testDemo(testDemo &&other)
    {
        std::cout << "调用移动构造函数 " << endl;
        p = other.p;
        other.p = nullptr;
    }
    auto operator=(testDemo &other)
    {
        std::cout << "调用拷贝=函数" << endl;
        delete p;
        p = new int(*other.p);
        return *this;
    }
    auto operator=(testDemo &&other)
    {
        std::cout << "调用移动=函数" << endl;
        delete p;
        p = other.p;
        other.p = nullptr;
        return *this;
    }
    ~testDemo()
    {
        if (p)
        {
            std::cout << "调用析构构造函数 " << *p << endl;

            delete p;
        }
        else
        {
            std::cout << "调用析构构造函数 " << endl;
        }
    }
    int *p;
};

auto gettest(int n)
{
    return testDemo(n);
}
template <bool A, class B, class C>
void tf()
{
    cout << "false" << endl;
}
template <class B, class C>
void tf<true, B, C>()
{
    cout << "true" << endl;
}
int main()
{
    tf<true, int, int>();
    tf<false, int, int>();
}