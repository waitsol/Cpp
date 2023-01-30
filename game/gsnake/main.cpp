// 参考A*算法讲解 http://t.csdn.cn/PCFnB

#include <iostream>
#include <algorithm>
#include <stack>
#include <set>
#include <map>
#include <random>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h> //用来反应错误警告的，后面会讲
#include <windows.h>
#include <time.h>
#include <list>
using namespace std;
class TCS
{
public:
    TCS(int m, int n) : e(time(0)), u(m * n), _m(m), _n(n), _g(m, vector<int>(n, 0))
    {

        // 上边界 y轴上移1
        for (int i = 0; i < n; i++)
        {
            gotoxy(bx + i, by - 1);
            printf("%c", '*');
        }
        // 下边界 y轴下移1

        for (int i = 0; i < n; i++)
        {
            gotoxy(bx + i, by + m + 1);
            printf("%c", '*');
        }
        // 左边
        for (int i = -1; i < m + 2; i++)
        {
            gotoxy(bx - 1, by + i);
            printf("%c", '*');
        }
        // right
        for (int i = -2; i < m + 1; i++)
        {
            gotoxy(bx + n, by + i + 1);
            printf("%c", '*');
        }
    }
    struct mapNode
    {
        int x, y;
        void set_xy(int xx, int yy)
        {
            x = xx;
            y = yy;
        }
        bool operator==(const mapNode &thr)
        {
            return x == thr.x && y == thr.y;
        }
        bool operator!=(const mapNode &thr)
        {
            return !(*this == thr);
        }
        bool operator<(const mapNode &r) const
        {
            if (x == r.x)
                return y < r.y;
            return x < r.x;
        }
    };

    struct A_node
    {

        int F = 0;
        int G = 0;
        int H = 0;
        int step = 0;
        mapNode cur;
        mapNode parent;

        bool operator<(const A_node &r) const
        {
            return F < r.F;
        }
    };
    mapNode getFood()
    {
        int x = u(e);
        int y = _m * _n;
        mapNode ret;

        while (1)
        {
            x %= y;
            ret.x = x / _n;
            ret.y = x % _n;
            if (_g[ret.x][ret.y] != 0)
                x++;
            else
                break;
        }
        return ret;
    }
    // 移动光标
    void gotoxy(int x, int y)
    {
        // 更新光标位置
        COORD pos;
        HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
        pos.X = x;
        pos.Y = y;
        SetConsoleCursorPosition(hOutput, pos);
        // 隐藏光标
        CONSOLE_CURSOR_INFO cursor;
        cursor.bVisible = false;
        cursor.dwSize = sizeof(cursor);
        SetConsoleCursorInfo(hOutput, &cursor);
    }
    void draw(mapNode dst, char c)
    {
        gotoxy(dst.y + bx, dst.x + by);
        printf("%c", c);
    }
    void run()
    {
        _step = 1;
        int len = _m * _n;
        mapNode h = getFood();
        _g[h.x][h.y] = 1;
        _q.push_back(h);
        mapNode pre;
        pre.x = pre.y = -1;
        int st = 200;
        while (_step < len)
        {
            mapNode food = getFood();
            auto route = A_search(h, food);
            int sz = 1;
            draw(food, '#');

            if (route.size())
            {
                ++_step;
                while (route.size())
                {
                    auto t = route.top();

                    // 新路径加入
                    _q.push_back(t);
                    draw(t, 'o');
                    if (pre.x != -1)
                    {
                        draw(pre, '.');
                    }
                    pre = t;
                    // 没吃到食物 就弹出
                    if (t != food)
                    {

                        auto pop = *_q.begin();
                        _q.pop_front();
                        _g[pop.x][pop.y] = 0;
                        // s如果头跟着尾部就不刷新了
                        if (pop != t)
                        {

                            draw(pop, ' ');
                        }
                    }
                    route.pop();
                    Sleep(st);
                }
            }
            else
            {
                gotoxy(30, 40);
                cout << "死路" << endl;
                return;
            }

            int idx = 1;
            for (auto &it : _q)
            {
                _g[it.x][it.y] = idx++;
            }
            st = max(80, st - 10);

            h = food;
        }
        gotoxy(30, 40);
    }

    stack<mapNode> A_search(mapNode src, mapNode dst)
    {

        set<mapNode> close_list;
        set<mapNode> open_list;
        map<mapNode, A_node> AF;
        // map模拟优先队列
        multiset<A_node> pq;

        stack<mapNode> ret;
        int dir[5] = {1, 0, -1, 0, 1};
        A_node start;
        start.step = 1;
        start.cur = src;
        start.G = 0;
        open_list.emplace(src);
        pq.emplace(start);
        AF[src] = start;
        while (pq.size())
        {
            A_node h = *pq.begin();
            pq.erase(pq.begin());
            open_list.erase(h.cur);
            close_list.emplace(h.cur);
            for (int i = 0; i < 4; i++)
            {
                mapNode next = h.cur;
                next.x += dir[i];
                next.y += dir[i + 1];
                if (next.x >= 0 && next.x < _m && next.y >= 0 && next.y < _n)
                {
                    // 蛇的身体无法到达
                    if (_g[next.x][next.y] - h.step > 0)
                        continue;

                    if (close_list.count(next))
                        continue;
                    if (open_list.count(next))
                    {
                        // 计算当前step值
                        int step = abs(dir[i]) + abs(dir[i + 1]) == 1 ? 10 : 14;
                        // 重新计算F值
                        auto &Fnode = AF.find(next)->second;

                        int F = Fnode.H + h.G + step;
                        if (F < Fnode.F)
                        {

                            auto iter = pq.find(Fnode);
                            while (iter != pq.end())
                            {
                                if (next == iter->cur)
                                {
                                    iter = pq.erase(iter);
                                    break;
                                }
                                else
                                {
                                    iter++;
                                }
                            }

                            if (next.x == 1 && next.y == 3)
                            {
                                cout << next.x << " " << next.y << endl;

                                cout << Fnode.F << " "
                                     << " " << Fnode.G << " " << Fnode.H << endl;
                            }
                            // 重新更新G
                            Fnode.F = F;
                            Fnode.G = h.G + step;
                            Fnode.parent = h.cur;
                            Fnode.step = h.step + 1;
                            pq.erase(Fnode);
                        }
                    }
                    else
                    {
                        A_node t;
                        t.cur = next;
                        t.parent = h.cur;
                        // 计算当前step值
                        int step = abs(dir[i]) + abs(dir[i + 1]) == 1 ? 10 : 14;
                        // 计算H
                        t.H = (abs(dst.x - next.x) + abs(dst.y - next.y)) * 10;
                        t.G = h.G + step;
                        t.F = t.G + t.H;
                        t.step = h.step + 1;
                        AF[next] = t;
                        pq.emplace(t);
                        open_list.emplace(next);

                        if (next == dst)
                        {

                            while (next != src)
                            {
                                ret.push(next);
                                next = AF.find(next)->second.parent;
                            }
                            return ret;
                        }
                    }
                }
            }
        }

        return ret;
    }

protected:
    int bx = 30;
    int by = 5;

private:
    vector<vector<int>> _g;
    int _step;
    int _m, _n;
    default_random_engine e;
    uniform_int_distribution<int> u;
    list<mapNode> _q;
};
int main()
{
    TCS t(40, 60);
    t.run();

    return 0;
}