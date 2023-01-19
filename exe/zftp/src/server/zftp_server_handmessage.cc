#include "zftp_server_handmessage.h"
#include "zlk_log.h"

#include <vector>
#include <string>
#include <list>
#include <fstream>
#include <sys/stat.h>
#include <unistd.h>
#include "zlk_file.h"
#include "zlk_ftp_server.h"
#include "zlk_string.h"

extern Server *g_server;
using namespace std;

string shell_cmd(string cmd, int id)
{

#ifndef WIN32
    char sep = '/';
#else
    char sep = '\\';

#endif
    vector<string> v = sp(cmd, sep);
    list<string> q;
    auto p = g_server->get_userinfo(id);
    for (auto x : p->_list)
    {
        q.push_back(x);
    }
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
    if (res.size() == 0)
        res = ".";
    return res;
}

zftp_server_handleMessage::zftp_server_handleMessage()
{
    m_mapFunc[zlkMsg::rm] = std::bind(&zftp_server_handleMessage::hand_rm, this, std::placeholders::_1, std::placeholders::_2);
    m_mapFunc[zlkMsg::ls] = std::bind(&zftp_server_handleMessage::hand_ls, this, std::placeholders::_1, std::placeholders::_2);
    m_mapFunc[zlkMsg::pull] = std::bind(&zftp_server_handleMessage::hand_pull, this, std::placeholders::_1, std::placeholders::_2);
    m_mapFunc[zlkMsg::push] = std::bind(&zftp_server_handleMessage::hand_push, this, std::placeholders::_1, std::placeholders::_2);
    m_mapFunc[zlkMsg::cd] = std::bind(&zftp_server_handleMessage::hand_cd, this, std::placeholders::_1, std::placeholders::_2);
    m_mapFunc[zlkMsg::mkdir] = std::bind(&zftp_server_handleMessage::hand_mkdir, this, std::placeholders::_1, std::placeholders::_2);
    m_mapFunc[zlkMsg::pwd] = std::bind(&zftp_server_handleMessage::hand_pwd, this, std::placeholders::_1, std::placeholders::_2);
};
void zftp_server_handleMessage::hand_ls(const zftp_message::Pakcet &packet, zftp_message::Pakcet &zlk_res)
{
    zftp_message::ls_msg msg;
    zftp_message::ls_response ls_res;
    ls_res.set_ok(false);
    if (msg.ParseFromString(packet.msgbody()))
    {
        // 防止越权
        UserInfo *pu = g_server->get_userinfo(packet.id());
        string cmd = shell_cmd(msg.data(), packet.id());
        struct stat st;
        if (stat(cmd.data(), &st) == 0)
        {

            if (st.st_mode & S_IFDIR)
            {
                ls_res.set_ok(true);
                auto res = getFiles(cmd);
                pu->_mapFileInfo.clear();
                for (int i = 0; i < res.size(); i++)
                {
                    auto node = ls_res.add_list();

                    struct stat st;
                    if (stat((cmd + "/" + res[i]).data(), &st) == 0)
                    {
                        node->set_type((st.st_mode & S_IFDIR) != 0);
                        node->set_size(st.st_size);
                    }

                    node->set_id(i + 1);
                    node->set_name(res[i]);
                    pu->_mapFileInfo[i + 1] = cmd + "/" + res[i];
                    DBG("res = %s", res[i].data());
                }

                DBG("%d", zlk_res.msgbody().size());
            }
            else
            {
                ls_res.set_error("file not dir");
            }
        }
        else
        {
            ls_res.set_error("file not exit");
        }
    }
    else
    {
        ERR("ParseFromString error");
    }
    zlk_res.set_msgbody(ls_res.SerializeAsString());
};
void zftp_server_handleMessage::hand_rm(const zftp_message::Pakcet &packet, zftp_message::Pakcet &zlk_res)
{
    zftp_message::rm_msg msg;
    zftp_message::response_msg res;
    if (msg.ParseFromString(packet.msgbody()))
    {
        string cmd = shell_cmd(msg.file_name(), packet.id());
        string shell = "rm ";
        if (msg.force())
            shell += " -f ";
        if (msg.recursion())
            shell += " -r ";
        if (access(cmd.data(), F_OK) != 0)
        {
            UserInfo *pu = g_server->get_userinfo(packet.id());

            auto iter = pu->_mapFileInfo.find(atoi(msg.file_name().data()));
            if (iter == pu->_mapFileInfo.end())
            {
                res.set_ok(false);
                res.set_error("file don't exit");
                zlk_res.set_msgbody(res.SerializeAsString());
                return;
            }
            else
            {
                cmd = iter->second;
            }
        }

        shell += cmd;
        int ret = system(shell.data());
        if (ret == 0)
        {
            res.set_ok(true);
        }
        else
        {
            res.set_ok(false);
            res.set_error(strerror(errno));
        }
    }
    else
    {
        ERR("ParseFromString error");
    }
    zlk_res.set_msgbody(res.SerializeAsString());
}
void zftp_server_handleMessage::hand_pull(const zftp_message::Pakcet &packet, zftp_message::Pakcet &zlk_res)
{
    zftp_message::pull_msg msg;
    zftp_message::pull_respon_msg res;
    res.set_ok(false);

    if (msg.ParseFromString(packet.msgbody()))
    {
        string cmd = shell_cmd(msg.name(), packet.id());
        bool buse = false;
        if (access(cmd.data(), F_OK) != 0)
        {
            UserInfo *pu = g_server->get_userinfo(packet.id());
            auto iter = pu->_mapFileInfo.find(atoi(msg.name().data()));
            if (iter == pu->_mapFileInfo.end())
            {
                res.set_data("file don't exit");
                zlk_res.set_msgbody(res.SerializeAsString());
                return;
            }
            else
            {
                buse = true;
                cmd = iter->second;
            }
        }
        struct stat st;
        if (stat(cmd.data(), &st) == 0 && (st.st_mode & S_IFREG))
        {
            ifstream ifs(cmd, ios::binary);
            if (ifs.is_open())
            {
                ifs.seekg(0, ifs.end);
                auto fos = ifs.tellg();
                ifs.seekg(0, ifs.beg);
                char *pbuf = new char[fos];
                DBG("file sz = %d", fos);
                if (pbuf != nullptr)
                {
                    ifs.read(pbuf, fos);
                    res.set_data(pbuf, fos);
                    res.set_ok(true);

                    if (!buse)
                    {
                        res.set_save_name(msg.dstname());
                    }
                    else
                    {
                        int idx = cmd.find_last_of('/');
                        if (idx != -1)
                        {
                            res.set_save_name(cmd.substr(idx + 1));
                        }
                        else
                        {
                            res.set_save_name(cmd);
                        }
                    }

                    delete[] pbuf;
                }
                else
                {
                    res.set_data("malloc failed");
                }

                ifs.close();
            }
        }
        else
        {
            res.set_data("open file failed");
        }
    }
    else
    {
        ERR("ParseFromString error");
        res.set_data("ParseFromString error");
    }
    zlk_res.set_msgbody(res.SerializeAsString());
}
void zftp_server_handleMessage::hand_push(const zftp_message::Pakcet &packet, zftp_message::Pakcet &zlk_res)
{
    zftp_message::push_msg msg;
    zftp_message::response_msg res;
    res.set_ok(false);
    if (msg.ParseFromString(packet.msgbody()))
    {
        // 防止越权
        string cmd = shell_cmd(msg.dst_name(), packet.id());
        struct stat st;
        if (stat(cmd.data(), &st) == 0 && (st.st_mode & S_IFDIR))
        {
            cmd += '/' + msg.src_name();
        }

        if (!msg.force())
        {
            if (access(cmd.data(), F_OK) == 0)
            {
                res.set_error("file is exit");
                goto Exit;
            }
        }

        ofstream ofs(cmd);
        if (ofs.is_open())
        {
            ofs.write(msg.data().data(), msg.data().length());
            ofs.close();
            res.set_ok(true);
        }
        else
        {
            res.set_error("open failed");
        }
    }
    else
    {
        ERR("ParseFromString error");
    }
Exit:
    zlk_res.set_msgbody(res.SerializeAsString());
};
void zftp_server_handleMessage::hand_cd(const zftp_message::Pakcet &packet, zftp_message::Pakcet &zlk_res)
{
    zftp_message::cd_msg msg;
    zftp_message::response_msg res;
    res.set_ok(false);

    if (msg.ParseFromString(packet.msgbody()))
    {
        string cmd = shell_cmd(msg.dir(), packet.id());
        struct stat st;

        DBG("cmd = %s", cmd.data());
        if (access(cmd.data(), F_OK) != 0)
        {
            UserInfo *pu = g_server->get_userinfo(packet.id());

            auto iter = pu->_mapFileInfo.find(atoi(msg.dir().data()));
            if (iter == pu->_mapFileInfo.end())
            {
                res.set_ok(false);
                res.set_error("file don't exit");
                zlk_res.set_msgbody(res.SerializeAsString());
                return;
            }
            else
            {
                cmd = iter->second;
            }
        }

        if (stat(cmd.data(), &st) == 0)
        {

            if (st.st_mode & S_IFDIR)
            {
                res.set_ok(true);
                auto *pu = g_server->get_userinfo(packet.id());
                pu->_list.clear();

                if (msg.dir().size() != 0)
                {
                    vector<string> v = sp(cmd, '/');

                    for (auto x : v)
                        pu->_list.push_back(x);

                    DBG(cmd.data());
                    DBG("sz = %d", pu->_list.size());
                }
                pu->_mapFileInfo.clear();
            }
            else
            {
                res.set_error(msg.dir() + " !S_IFDIR" + " is " + to_string(st.st_mode));
            }
        }
        else
        {
            res.set_error(msg.dir() + "is not exit");
        }
    }
    else
    {
        ERR("ParseFromString error");
        res.set_error("ParseFromString error");
    }
    zlk_res.set_msgbody(res.SerializeAsString());
}
void zftp_server_handleMessage::hand_mkdir(const zftp_message::Pakcet &packet, zftp_message::Pakcet &zlk_res)
{
    zftp_message::mkdir_msg msg;
    zftp_message::response_msg res;
    res.set_ok(false);

    if (msg.ParseFromString(packet.msgbody()))
    {
        string cmd = shell_cmd(msg.data(), packet.id());
        string shell = "mkdir " + cmd;
        if (system(shell.data()) == 0)
        {
            res.set_ok(true);
        }
        else
        {
            res.set_error(strerror(errno));
        }
    }
    else
    {
        ERR("ParseFromString error");
        res.set_error("ParseFromString error");
    }
    zlk_res.set_msgbody(res.SerializeAsString());
}

void zftp_server_handleMessage::hand_pwd(const zftp_message::Pakcet &packet, zftp_message::Pakcet &zlk_res)
{
    zftp_message::response_msg res;
    res.set_ok(true);
    res.set_error(shell_cmd("", packet.id()));
    zlk_res.set_msgbody(res.SerializeAsString());
}