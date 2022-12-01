#ifndef _BITMAP_H
#define _BITMAP_H
#include <string.h>
using namespace std;
class Bitmap
{
public:
    using uint64 = unsigned long long;
    Bitmap(uint64 size)
    {
        mSize = (size + ALIGN - 1) & ~(ALIGN - 1); //将size上升至ALIGN的整数倍
        uint64 n = mSize / ALIGN;
        p = new uint64[n];
        memset(p, 0, n * sizeof(uint64));
    }
    bool Setn(uint64 n)
    {
        if (n >= mSize)
            return false;
        int idx = n / 64;
        int mod = n % 64;
        p[idx] |= (1 << mod);
        return true;
    }
    bool Getn(uint64 n)
    {
        if (n >= mSize)
            return false;
        int idx = n / 64;
        int mod = n % 64;
        return p[idx] & (1 << mod);
    }
    ~Bitmap()
    {
        if (p)
        {
            delete[] p;
        }
    }

private:
    static const uint64 ALIGN = 64;
    uint64 *p;
    uint64 mSize;
};
#endif