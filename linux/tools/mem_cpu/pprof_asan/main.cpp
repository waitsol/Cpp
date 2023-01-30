#include <iostream>
#include <string.h>
#include <gperftools/profiler.h>
#include <gperftools/heap-checker.h>

using namespace std;
void f1()
{
    cout << __func__ << endl;
    int *a = new int[10];
    memset(a, 0, sizeof(int) * 10);
}
void f2()
{
    cout << __func__ << endl;

    int *a = new int[20];
    memset(a, 0, sizeof(int) * 20);
    int *b = new int[20];
    memset(b, 0, sizeof(int) * 20);
    f1();
}
void f3()
{
    int *a = new int[20];
    memset(a, 0, sizeof(int) * 20);
    cout << __func__ << endl;

    f1();
}
// bool memory_check()
// {
//     HeapLeakChecker::NoGlobalLeaks();
//     return true;
// }
// 直接打印是否有内存泄漏 不用等待程序结束
bool memory_check(void *arg)
{
    HeapLeakChecker::NoGlobalLeaks();
    return 1;
}
int main()
{
    int idx = 1;
    int a;
    while (cin >> a)
    {
        if (a == 0)
            break;
        if (a == 1)
            f1();
        else if (a == 2)
            f2();
        else if (a == 3)
            f3();
        else if (a == 4)
            memory_check(0);
    }
    return 0;
}