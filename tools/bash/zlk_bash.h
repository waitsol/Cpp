#ifndef ZLK_BASH
#define ZLK_BASH
#include <stdio.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <termios.h>
#include <signal.h>
#include <stdlib.h>
#include <algorithm>
#include <string>
#include <string.h>
#include <list>
#include <vector>
using namespace std;
class DicTree
{
    using self = DicTree *;

public:
    DicTree(unsigned char k = 0)
    {
        isKey = false;
        val = k;
    }
    ~DicTree()
    {
        uninit(this);
    }

    void insert(const string &s)
    {
        self c = this;
        for (const auto &x : s)
        {
            auto next = find(c, x);
            if (next == nullptr)
            {
                next = new DicTree(x);
                c->child.push_back(next);
            }
            c = next;
        }
        c->isKey = true;
    }
    vector<string> check(const char *data, int begin, int end)
    {
        self c = this;
        string tmp;
        tmp.reserve(100);
        for (int i = begin; i < end; i++)
        {
            auto next = find(c, data[i]);
            if (next == nullptr)
            {
                return {};
            }
            tmp += data[i];
            c = next;
        }
        vector<string> res;
        res.reserve(10);

        getchild(c, res, tmp);
        return res;
    }

private:
    self find(self th, unsigned char c)
    {
        for (auto &iter : th->child)
        {
            if (iter->val == c)
                return iter;
        }
        return nullptr;
    }
    void uninit(self p)
    {
        auto iter = p->child.begin();
        while (iter != p->child.end())
        {
            uninit(*iter);
            delete *iter;
            iter = p->child.erase(iter);
        }
    }
    void getchild(self th, vector<string> &out, string &s)
    {
        for (auto &iter : th->child)
        {
            s += iter->val;
            getchild(iter, out, s);
            s.pop_back();
        }
        if (th->isKey)
            out.push_back(s + '\n'); // 这里加吧
    }

private:
    list<DicTree *> child;
    unsigned char val;
    bool isKey;
};
inline void clear_upcache()
{
    char buf[] = {65, 27, 91, 66};
    ::write(STDOUT_FILENO, buf, 4);
}
inline void clear_downcache()
{
    char buf[] = {66};
    ::write(STDOUT_FILENO, buf, 1);
}
class zlk_bashBuffer
{
public:
    zlk_bashBuffer() : buffer(new char[1024]), idx(0), end(0) {}
    ~zlk_bashBuffer()
    {
        if (buffer)
        {
            delete[] buffer;
            buffer = nullptr;
        }
    }
    zlk_bashBuffer(const zlk_bashBuffer &r)
    {

        buffer = new char[1024];

        memcpy(buffer, r.buffer, r.end);
        idx = end = r.end;
    };
    zlk_bashBuffer &operator=(const zlk_bashBuffer &r)
    {

        if (buffer == nullptr)
            buffer = new char[1024];

        if (buffer != r.buffer) //=自己的情况
            memcpy(buffer, r.buffer, r.end);

        idx = end = r.end;
        return *this;
    }
    bool left()
    {
        idx = idx - 1;
        if (idx != -1)
            return true;
        idx = 0;
        return false;
    };
    bool right()
    {
        idx = idx + 1;
        if (idx <= end)
            return true;
        idx = end;
        return false;
    };
    void write(char c)
    {
        if (idx < end)
        {
            memmove(buffer + idx + 1, buffer + idx, end - idx);
        }

        buffer[idx++] = c;
        ++end;
        ::write(STDOUT_FILENO, wdata(), wlenth());

        for (int i = idx; i < end; i++)
        {
            ::write(STDOUT_FILENO, &LEFTVAL, 3);
        }
    }
    void del()
    {
        if (idx == 0)
            return;

        char c = ' ';
        // 左移一个
        ::write(STDOUT_FILENO, &LEFTVAL, 3);
        // abcd ->abc
        // abcd ->acd
        ::write(STDOUT_FILENO, wdata() + 1, wlenth() - 1); // idx开始写
        ::write(STDOUT_FILENO, &c, 1);                     // 最后写一个空格
        ::write(STDOUT_FILENO, &LEFTVAL, 3);               // 左移到末尾 最后那个空格是舍弃的
        if (idx < end)
        {
            memmove(buffer + idx - 1, buffer + idx, end - idx); // 光标不在末尾要移动数据
        }
        --end;
        --idx;
        for (int i = idx; i < end; i++)
        {
            ::write(STDOUT_FILENO, &LEFTVAL, 3);
        }
    }
    void clear()
    {
        char c = ' ';

        for (int i = idx; i < end; i++)
        {
            ::write(STDOUT_FILENO, &c, 1);
        }
        for (int i = 0; i <= end; i++)
        {
            ::write(STDOUT_FILENO, &c, 1);
            ::write(STDOUT_FILENO, &LEFTVAL, 3);
            ::write(STDOUT_FILENO, &LEFTVAL, 3);
        }
        ::write(STDOUT_FILENO, &LEFTVAL, 3);
    }
    void reset()
    {
        if (!buffer)
            buffer = new char[1024];
        idx = end = 0;
    }
    void full()
    {
        ::write(STDOUT_FILENO, buffer, end);
        for (int i = idx; i < end; i++)
        {
            ::write(STDOUT_FILENO, &LEFTVAL, 3);
        }
    }
    bool tab(int &out_begin, int &out_end)
    {
        // 不在末尾并且不是最后
        if (idx != end && buffer[idx] != ' ')
            return false;
        out_end = idx;
        out_begin = idx - 1; // 前一个开始检查 当前这个检查过了
        // 往前找 找到空格 或者开始
        while (out_begin >= 0 && buffer[out_begin--] != ' ')
            ;
        // 上一个满足要求的
        if (out_begin == -1)
            out_begin = 0;
        else
            out_begin += 2;
        if (out_begin >= out_end)
            return false;
        return true;
    }
    void to_end()
    {
        for (int i = idx; i < end; i++)
        {
            ::write(STDOUT_FILENO, &RIGHTVAL, 3);
        }
    }
    char *c_str() { return buffer; }
    string to_string()
    {
        return string(buffer, end);
    }

private:
    inline char *wdata() { return buffer + idx - 1; };
    inline int wlenth() { return end - idx + 1; };
    // 1423
private:
    char *buffer = 0;
    int idx;
    int end;
    inline static const int LEFTVAL = 4479771;  //  char buf[10] = { 27,91,68 };左符号编码转int
    inline static const int RIGHTVAL = 4414235; //  char buf[10] = { 27,91,67 };右符号编码转int
};
class zlk_bash_cache
{
public:
    zlk_bash_cache() : iter(plist.end()) {}
    void add(const zlk_bashBuffer &b)
    {
        plist.push_back(b);
        if (plist.size() > 20)
            plist.pop_front();
        iter = plist.end();
    }
    zlk_bashBuffer *up()
    {
        if (iter != plist.begin())
        {
            --iter;
            return &*iter;
        }
        return nullptr;
    }
    zlk_bashBuffer *down()
    {
        if (iter != plist.end() && ++iter != plist.end())
        {
            return &*iter;
        }
        return nullptr;
    }

    list<zlk_bashBuffer> plist;
    decltype(plist.end()) iter;
};
class zlk_bask
{
public:
    using deal_func = void (*)(string);
    zlk_bask(deal_func pf) : pfunc(pf) {}
    void run()
    {
        int fd;
        fd_set readfds;
        unsigned char c;
        int result;
        struct termios ts, ots;

        fd = STDIN_FILENO;

        tcgetattr(fd, &ts);
        ots = ts;
        ts.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(fd, TCSAFLUSH, &ts);
        FD_ZERO(&readfds);
        FD_SET(0, &readfds);
        zlk_bashBuffer buf;
        zlk_bash_cache cache;
        bool esc = false;
        bool dir = false;
        while (1)
        {
            result = pselect(1, &readfds, NULL, NULL, NULL, NULL);

            result = read(fd, &c, 1);

            if (result != 1)
            {
                break;
            }
            // printf("%d\n", c);
            // continue;
            switch (c)
            {
            case 27:
            {
                write(STDOUT_FILENO, &c, 1);
                esc = true;
                break;
            }
            case 91:
            {
                if (esc)
                {
                    dir = true;
                    write(STDOUT_FILENO, &c, 1);
                    esc = false;
                }
                break;
            }
            case 68: // left
            {
                dir = false;

                if (buf.left())
                    write(STDOUT_FILENO, &c, 1);
                break;
            }
            case 67: // right
            {
                dir = false;

                if (buf.right())
                    write(STDOUT_FILENO, &c, 1);
                break;
            }
            case 65: // up
            {
                clear_upcache();
                dir = false;
                auto ret = cache.up();
                if (ret != nullptr)
                {
                    buf.clear();
                    buf = *ret;
                    buf.full();
                }
                break;
            }
            case 66: // down
            {
                clear_downcache();
                dir = false;
                auto ret = cache.down();
                buf.clear();
                if (ret != nullptr)
                {
                    buf = *ret;
                    buf.full();
                }
                else
                {
                    // 重新构造一个
                    buf = zlk_bashBuffer();
                }
                break;
            }
            case 8: //<-
            {
                buf.del();

                break;
            }
            case 9: // tab
            {
                int begin = 0, end = 0;
                if (buf.tab(begin, end))
                {
                    auto vec = dictree.check(buf.c_str(), begin, end);
                    if (vec.size() > 1)
                    {
                        buf.to_end();
                        write(STDOUT_FILENO, "\n", 1);

                        for (auto &x : vec)
                        {
                            write(STDOUT_FILENO, x.data(), x.length());
                        }
                        buf.full();
                    }
                    else if (vec.size() == 1)
                    {
                        // 最后一个是\n不用
                        for (int i = end - begin; i < vec[0].size() - 1; i++)
                        {
                            buf.write(vec[0][i]);
                        }
                    }
                }

                break;
            }
            case 10:
            {
                cache.add(buf);
                buf.to_end();
                write(STDOUT_FILENO, "\n", 1);
                if (pfunc != nullptr)
                {
                    pfunc(buf.to_string());
                }
                buf.reset();
                break;
            }
            default:
                buf.write(c);
                break;
            }
        }
    }
    void regist(string s)
    {
        dictree.insert(s);
    }

private:
    deal_func pfunc;
    DicTree dictree;
};

#endif