// 参考A*算法讲解 http://t.csdn.cn/PCFnB

#include <iostream>
#include <algorithm>
#include <stack>
#include <set>
#include <map>
#include <vector>
using namespace std;
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
    mapNode cur;
    mapNode parent;

    bool operator<(const A_node &r) const
    {
        return F < r.F;
    }
};

stack<mapNode> A_search(const vector<vector<int>> &g, mapNode src, mapNode dst)
{
    int n = g.size();
    int m = g[0].size();
    set<mapNode> close_list;
    set<mapNode> open_list;
    map<mapNode, A_node> AF;
    // map模拟优先队列
    multiset<A_node> pq;

    stack<mapNode> ret;
    int dir[8][2];
    {
        int idx = 0;
        for (int i = -1; i <= 1; i++)
        {
            for (int j = -1; j <= 1; j++)
            {
                if (i == 0 && j == 0)
                    continue;
                dir[idx][0] = i;
                dir[idx][1] = j;
                idx++;
            }
        }
    }
    A_node start;
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
        for (int i = 0; i < 8; i++)
        {
            mapNode next = h.cur;
            next.x += dir[i][0];
            next.y += dir[i][1];
            if (next.x >= 0 && next.x < n && next.y >= 0 && next.y < m)
            {
                if (g[next.x][next.y] == 0)
                    continue;

                // 如果是不能对强穿
                if (abs(dir[i][0]) + abs(dir[i][1]) == 2)
                {
                    if (g[next.x][h.cur.y] == 0 || g[h.cur.x][next.y] == 0)
                        continue;
                }

                if (close_list.count(next))
                    continue;
                if (open_list.count(next))
                {
                    // 计算当前step值
                    int step = abs(dir[i][0]) + abs(dir[i][1]) == 1 ? 10 : 14;
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
                        pq.erase(Fnode);
                    }
                }
                else
                {
                    A_node t;
                    t.cur = next;
                    t.parent = h.cur;
                    // 计算当前step值
                    int step = abs(dir[i][0]) + abs(dir[i][1]) == 1 ? 10 : 14;
                    // 计算H
                    t.H = (abs(dst.x - next.x) + abs(dst.y - next.y)) * 10;
                    t.G = h.G + step;
                    t.F = t.G + t.H;
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

int main()
{

    vector<vector<int>> g = {
        {1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 0, 1, 1, 1},
        {1, 1, 1, 1, 0, 1, 1, 1},
        {1, 1, 1, 1, 0, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1},
    };
    mapNode src, dst;
    src.x = 2;
    src.y = 2;
    dst.x = 2;
    dst.y = 6;

    auto ret = A_search(g, src, dst);
    while (ret.size())
    {
        cout << ret.top().x << " " << ret.top().y << endl;
        ret.pop();
    }
    return 0;
}