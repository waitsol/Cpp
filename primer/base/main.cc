/*记录一些常用容易记错的东西*/

int main()
{
    // 数组方面
    {
        int arr[10];
        // p是一个指针 指向10个大小的数组
        int(*p)[10] = &arr;
        // p2是一个引用
        int(&p2)[10] = arr;
        // 利用模板求数组大小 参数是一个数组引用,也可以用*代替&不过传的时候需要取地址 麻烦
        /*template <typename T, int n>
        inline constexpr int arr_size(T(*p)[n])
        {
            cout << typeid(p).name() << endl;
            cout << p[0] << endl;
            return n;
        }
        */
    }
    // const

    {
        int x = 3;
        // p1和p2一个意思  p3是常量 指向的地址可以赋值
        const int *p1 = &x;
        int const *p2 = &x;

        int *const p3 = &x;
    }
    // typedef
    {
        typedef char *pchar;
        const char *p1 = "asdas";
        char *p2 = nullptr;
        //!=const char* p1= "asdas";
        const pchar p3 = p2;
        // error  p3 = p2;  const修饰的是pchar pchar是个指针 所以他指向的是char* 而不是const char*所以不可以再次赋值
    }
}
