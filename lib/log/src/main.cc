#include "zlk_log.h"
#include <set>
#include <map>
#define INF(pszFormat, args...) zlk_log::getInstance().write(zlk_logmode(int(zlk_logmode_debug) | int(zlk_logmode_line)), pszFormat, __FILE__, __func__, __LINE__, ##args)
int sz = 3000000;
int a[13] = {0};
void debuglog(int n)
{
    for (int i = 1; i <= sz; i++)
    {
        INF("debuglog n =  %d,%d", n, i);
        // zlk_log::getInstance().write(zlk_logmode_debug, "debuglog n =  %d,%d", n, i);

        a[n] = i;
    }
}
void test()
{
    ifstream ifs("aa");
    string s;
    int c = 0;
    map<int, set<int>> m;

    while (getline(ifs, s, '\n'))
    {
        ++c;
        int a, b;
        sscanf(s.data(), "[zlk_logmode_debug]:debuglog n =  %d,%d", &a, &b);
        m[a].emplace(b);
    }
    for (auto x : m)
    {
        cout << x.second.size() << endl;
    }
    for (auto x : m)
    {
        vector<bool> v(sz, false);
        for (auto xx : x.second)
            v[xx] = true;
        for (int i = 0; i < sz; i++)
            if (v[i] == false)
                cout << i << " ";
        cout << endl;
    }
}
int main()
{
    // test();
    // return 0 ;

    zlk_log::getInstance().init("aa", 2048, true);

    zlk_log::getInstance().t = 0;
    thread *pt[40];
    int n = 12;
    auto start = GetTickCount64();
    for (int i = 0; i < n; i++)
    {
        pt[i] = new thread(std::bind(debuglog, i));
    }
    new thread([]()
               {
            while(1)
            {
                sleep(1);
                for(int i=0;i<12;i++)
                    cout<<a[i]<<" ";
                cout<<endl;
            } });
    for (int i = 0; i < n; i++)
    {
        pt[i]->join();
        printf("i = %d over\n", i);
        delete pt[i];
    }
    sleep(2);

    zlk_log::getInstance().debug();

    cout << "over use time " << (zlk_log::getInstance().t - start) << endl;
    zlk_log::getInstance().uninit();
    system("wc -l aa");
}