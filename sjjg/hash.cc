// hash.cc.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
using namespace std;
using int64 = long long;
using int32 = long;
using uint32 = unsigned long;
using uint64 = unsigned long long;
struct dictEntry
{
    void *key;
    union
    {
        void *val;
        uint64 u64;
        int64 s64;
        double d;
    };
    dictEntry *next;
};
template <class T>
int64 hashcode(T key);
template <class T>
dictEntry *CreateDictEntry(T val)
{
    dictEntry *p = new dictEntry;
    p->val = new T(val);
    return p;
}
template <>
dictEntry *CreateDictEntry(double val)
{
    dictEntry *p = new dictEntry;
    p->d = val;
    return p;
}
template <>
dictEntry *CreateDictEntry(int64 val)
{
    dictEntry *p = new dictEntry;
    p->s64 = val;
    return p;
}
template <>
dictEntry *CreateDictEntry(int32 val)
{
    dictEntry *p = new dictEntry;
    p->s64 = val;
    return p;
}
dictEntry *CreateDictEntry(uint64 val)
{
    dictEntry *p = new dictEntry;
    p->u64 = val;
    return p;
}
template <>
dictEntry *CreateDictEntry(uint32 val)
{
    dictEntry *p = new dictEntry;
    p->u64 = val;
    return p;
}
template <class T>
T GetDictEntryVal(dictEntry *p)
{
    return *p->val;
}
template <>
int64 GetDictEntryVal<int64>(dictEntry *p)
{
    return p->s64;
}
template <>
int32 GetDictEntryVal<int32>(dictEntry *p)
{
    return p->s64;
}
template <>
uint64 GetDictEntryVal<uint64>(dictEntry *p)
{
    return p->u64;
}
template <>
uint32 GetDictEntryVal<uint32>(dictEntry *p)
{
    return p->u64;
}
template <>
double GetDictEntryVal<double>(dictEntry *p)
{
    return p->d;
}
template <class>
void FreeDictEntry(dictEntry *p)
{
    if (p->val)
        delete p->val;
    if (p->key)
        delete p->key;
    delete p;
}
template <class Tk, class TV>
class Hashtable
{
public:
    Hashtable(int64 size = 53, double _op = 0.9) : op(_op)
    {

        sz = search_prime(size);
        used = 0;
        table = new dictEntry *[sz];
    }
    bool insert(Tk key, TV val)
    {
        resize();
        dictEntry *p = CreateDictEntry(val);
        p->key = new Tk(key);
        return _insert(table, p);
    }

private:
    bool _insert(dictEntry **dict, dictEntry *pde)
    {
        int64 hk = hashcode(static_cast<Tk>(*pde->key));
        int idx = hk % sz;
        if (table[idx] == nullptr)
            table[idx] = pde;
        else
        {
            dictEntry *p = table[idx];
            while (p->next != nullptr)
            {
                if (static_cast<Tk>(*p->key) == static_cast<Tk>(*pde->key))
                {
                    FreeDictEntry<TV>(pde);

                    return false;
                }
                p = p->next;
            }
            p->next = pde;
        }
        return true;
    }

private:
    uint64 search_prime(uint64 n)
    {
        int left = 0;
        int right = __stl_num_primes - 1;
        if (n >= __stl_prime_list[right])
            return __stl_prime_list[right];
        while (left < right)
        {
            int mid = (left + right) / 2;
            if (__stl_prime_list[mid] > n)
                right = mid;
            else if (__stl_prime_list[mid] < n)
                left = mid + 1;
            else
                return n;
        }
        return __stl_prime_list[left];
    }
    void resize()
    {
        if (double(used) / sz > op && sz != __stl_prime_list[__stl_num_primes - 1])
        {
            int64 tsz = sz;
            sz = search_prime(sz + 1);
            dictEntry **tmp = new dictEntry *[sz];
            for (int i = 0; i < tsz; i++)
            {
                dictEntry *p = table[i];
                dictEntry *pnext = nullptr;
                while (p != nullptr)
                {
                    pnext = p->next;
                    p->next = nullptr;
                    // p节点不应该有下一个节点
                    _insert(tmp, p);
                    p = pnext;
                }
            }
            delete[] table;
            table = tmp;
        }
    }

private:
    const double op;
    int64 used;
    int64 sz;
    dictEntry **table;

    enum
    {
        __stl_num_primes = 28
    };

    const uint64 __stl_prime_list[__stl_num_primes] =
        {
            53ul, 97ul, 193ul, 389ul, 769ul,
            1543ul, 3079ul, 6151ul, 12289ul, 24593ul,
            49157ul, 98317ul, 196613ul, 393241ul, 786433ul,
            1572869ul, 3145739ul, 6291469ul, 12582917ul, 25165843ul,
            50331653ul, 100663319ul, 201326611ul, 402653189ul, 805306457ul,
            1610612741ul, 3221225473ul, 4294967291ul};
};
template <>
int64 hashcode(int64 key)
{
    return key;
}
int main()
{
    Hashtable<int64, int64> s;

    return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧:
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
