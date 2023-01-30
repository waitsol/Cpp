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
class Maze
{
private:
    enum strong
    {
        QUAN = 15,
        UP = 1,    // 0
        RIGHT = 2, // 1
        LEFT = 4,  // 2
        DOWN = 8,  // 3

    };
    // 15=1 1 1 1 分别代表上下左右的值
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
    struct DIR
    {
        int i, x, y;
        DIR(int ii, int xx, int yy) : i(ii), x(xx), y(yy) {}
    };

public:
    void generaG(int m, int n)
    {
        _g = vector<vector<int>>(m, vector<int>(n, QUAN));
        _n = n;
        _m = m;
        dfs(_g, 0, 0);
    }
    void print()
    {
        for (int y = 0; y < _m; y++)
        {
            for (int x = 0; x < _n; x++)
            {
                //|1|2|
                //|3|4|
                //|5|6|
                /*	cout << _g[x][y] << endl;
                    continue;*/
                if (_g[y][x] & UP)
                {
                    draw((x + 1) * 2, (y + 1) * 2 - 1, "-");
                }
                if (_g[y][x] & DOWN)
                {
                    draw((x + 1) * 2, (y + 1) * 2 + 1, "-");
                }
                if (_g[y][x] & LEFT)
                {
                    draw((x + 1) * 2 - 1, (y + 1) * 2, "|");
                }
                if (_g[y][x] & RIGHT)
                {
                    draw((x + 1) * 2 + 1, (y + 1) * 2, "|");
                }
            }
        }
        draw(0, _m + 20, "");
    }

    void toEnd()
    {
        Sleep(1000);
        mapNode src, dst;
        src.set_xy(0, 0);
        dst.set_xy(_m - 1, _n - 1);
        auto stk = A_search(src, dst);
        draw(src.x * 2 + 2, src.y * 2 + 2, "*");
        Sleep(700);
        while (stk.size())
        {
            dst = stk.top();
            stk.pop();
            // 这里得倒过来 这里的x指的是在第几行 是y坐标
            draw(dst.y * 2 + 2, dst.x * 2 + 2, "*");
            Sleep(300);
        }
        draw(0, 10 + _m, "");
    }

private:
    void dfs(vector<vector<int>> &g, int x, int y)
    {
        /*
            UP = 1, //0
            RIGHT = 2,//1
            LEFT = 4,//2
            DOWN = 8,//3
        */
        // 上 右 左 下s
        static vector<DIR> dir{{0, -1, 0}, {1, 0, 1}, {2, 0, -1}, {3, 1, 0}};

        static default_random_engine e(time(0));
        // cout << " dfs " << x << " " << y << endl;
        shuffle(dir.begin(), dir.end(), e);
        for (int i = 0; i < 4; i++)
        {
            int k = dir[i].i;
            int xx = x + dir[i].x;
            int yy = y + dir[i].y;
            //		cout << xx << " " << yy << endl;
            if (xx >= 0 && xx < _m && yy >= 0 && yy < _n)
            {
                if (g[xx][yy] == QUAN)
                {
                    g[x][y] -= (1 << k);
                    g[xx][yy] -= (1 << (3 - k));
                    dfs(g, xx, yy);
                }
            }
        }
    }

    stack<mapNode> A_search(mapNode src, mapNode dst)
    {

        set<mapNode> close_list;
        set<mapNode> open_list;
        map<mapNode, A_node> AF;
        // map模拟优先队列
        multiset<A_node> pq;

        stack<mapNode> ret;
        static vector<DIR> dir{{0, -1, 0}, {1, 0, 1}, {2, 0, -1}, {3, 1, 0}};
        // int dir[5] = { 1, 0, -1, 0, 1 };
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
            //	cout << h.cur.x << " " << h.cur.y << endl;
            for (int i = 0; i < 4; i++)
            {
                mapNode next = h.cur;
                next.x += dir[i].x;
                next.y += dir[i].y;
                if (next.x >= 0 && next.x < _m && next.y >= 0 && next.y < _n)
                {

                    if ((_g[next.x][next.y] & (1 << (3 - i))) != 0)
                        continue;

                    if (close_list.count(next))
                        continue;
                    if (open_list.count(next))
                    {
                        // 计算当前step值
                        int step = abs(dir[i].x) + abs(dir[i].y) == 1 ? 10 : 14;
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

                            // if (next.x == 1 && next.y == 3)
                            //{
                            ////	cout << next.x << " " << next.y << endl;

                            //	cout << Fnode.F << " "
                            //	//	<< " " << Fnode.G << " " << Fnode.H << endl;
                            //}
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
                        int step = abs(dir[i].x) + abs(dir[i].y) == 1 ? 10 : 14;
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
    void draw(int x, int y, const char *c)
    {
        gotoxy(x + bx, y + by);
        printf("%s", c);
    }

private:
    vector<vector<int>> _g;
    int _m, _n;
    int bx = 30;
    int by = 5;
};
int main()
{
    Maze ma;
    ma.generaG(40, 40);
    ma.print();
    ma.toEnd();
    return 0;
}