#include "zftp_server_handmessage.h"
#include "zlk_log.h"

#include <vector>
#include <string>
#include <list>
#include "zlk_file.h"
vector<string> sp(const string &s, char c = ' ')
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
string shell_cmd(string cmd)
{

#ifndef WIN32
    char sep = '/';
#else
    char sep = '\\';

#endif
    vector<string> v = sp(cmd, sep);
    list<string> q;
    for (auto &s : v)
    {
        if (s == ".")
            continue;
        if (s == "..")
        {
            if (q.size())
                q.pop_back();
        }
        else
        {
            q.push_back(s);
        }
    }
    string res;
    for (auto &s : q)
    {
        res += s;
        res += sep;
    }
    if (res.size())
        res.pop_back();
    return res;
}

zftp_server_handleMessage::zftp_server_handleMessage()
{
    m_mapFunc[zlkMsg::rm] = std::bind(&zftp_server_handleMessage::hand_rm, this, std::placeholders::_1, std::placeholders::_2);
    m_mapFunc[zlkMsg::ls] = std::bind(&zftp_server_handleMessage::hand_ls, this, std::placeholders::_1, std::placeholders::_2);
};
void zftp_server_handleMessage::hand_ls(const zftp_message::Pakcet &packet, zftp_message::Pakcet &zlk_res)
{
    zftp_message::ls_msg msg;
    if (msg.ParseFromString(packet.msgbody()))
    {
        // 防止越权
        string cmd = shell_cmd(msg.data());
        auto res = getFiles(cmd);
        zftp_message::ls_response ls_res;
        for (int i = 0; i < res.size(); i++)
        {
            auto node = ls_res.add_list();
            node->set_id(i + 1);
            node->set_name(res[i]);
            DBG("res = %s", res[i].data());
        }
        zlk_res.set_msgbody(ls_res.SerializeAsString());
        DBG("%d", zlk_res.msgbody().size());
    }
    else
    {
        DBG("ParseFromString error");
    }
};
void zftp_server_handleMessage::hand_rm(const zftp_message::Pakcet &packet, zftp_message::Pakcet &zlk_res)
{
}
void zftp_server_handleMessage::hand_pull(const zftp_message::Pakcet &packet, zftp_message::Pakcet &zlk_res)
{
}
void zftp_server_handleMessage::hand_push(const zftp_message::Pakcet &packet, zftp_message::Pakcet &zlk_res)
{
}