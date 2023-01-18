#ifndef __ZLK_STRING
#define __ZLK_STRING
#include <vector>
#include <string>
using namespace std;
static vector<string> sp(const string &s, char c = ' ')
{
    vector<string> r;
    int b = -1;
    for (int i = 0; i < s.size(); i++)
    {
        if (s[i] == c)
        {
            if (i - b != 1)
            {
                r.push_back(s.substr(b + 1, i - 1 - b));
            }
            b = i;
        }
    }
    if (b != s.size() - 1)
        r.push_back(s.substr(b + 1, s.size() - 1 - b));
    return r;
}

#endif