#include <iostream>
#include "lua_load.h"
#include "zlk_ftp_client.h"
#include "boost_socket.h"
#include "zlk_message.pb.h"
#include "zftp_client_handmessage.h"
#include "zlk_string.h"
#include "zlk_bash.h"
#include <map>
#ifdef WIN32
#define sep '\\'
#else
#define sep '/'
#endif
Client *g_client;
std::map<string, void (*)(vector<string> &)> pf;

void Client::regist()
{
    // 消息回调依赖的实体  程序死亡结束 不用管理指针
    regist(new zftp_client_handleMessage);
}
// 注册消息
void Client::regist(zlk_messagehandle *pz)
{
    for (auto x : pz->m_mapFunc)
    {
        m_mapFunc[x.first] = x.second;
    }
}
void Client::_send_message(const zftp_message::Pakcet packet)
{
    string s;
    if (packet.SerializeToString(&s))
    {
        DBG("send size = %d", s.size());
        //  printf("send_msg size = %d\n", s.size());
        m_connect->send_message(s.data(), s.size());
    }
    else
    {
        ERR("send_msg error");
    }
}
void Client::send_msg(const zftp_message::Pakcet &packet)
{
    m_io_service_pool.get_acceptor_io_service().post(std::bind(&Client::_send_message, this, packet));
}
void Client::handle_message(zftp_message::Pakcet &packet, zftp_message::Pakcet &packetResponse)
{
    auto iter = m_mapFunc.find(zlkMsg(packet.msgid()));
    if (iter != m_mapFunc.end())
    {
        DBG("run func msgid = %d", packet.msgid());
        iter->second(packet, packetResponse);
    }
    else
    {
        ERR("not find msg id = %d", packet.msgid());
    }
}
void cmd_ls(vector<string> &v)
{
    zftp_message::Pakcet packet;
    zftp_message::ls_msg ls;
    if (v.size() >= 2)
    {
        ls.set_data(v[1]);
    }
    else
    {
        ls.set_data(".");
    }
    packet.set_msgid(int(zlkMsg::ls));
    string str;
    ls.SerializeToString(&str);
    packet.set_msgbody(str);
    g_client->send_msg(packet);
}
void cmd_rm(vector<string> &v)
{
    int n = v.size();

    zftp_message::Pakcet packet;
    zftp_message::rm_msg rm;
    packet.set_msgid(int(zlkMsg::rm));

    for (int i = 1; i < n; i++)
    {
        if (v[i] == "-f")
        {
            rm.set_force(true);
        }
        else if (v[i] == "-r")
        {
            rm.set_recursion(true);
        }
    }
    for (int i = 1; i < n; i++)
    {
        if (v[i] != "-f" && v[i] != "-r")
        {
            rm.set_file_name(v[i]);
            packet.set_msgbody(rm.SerializeAsString());
            g_client->send_msg(packet);
        }
    }
}
void cmd_pull(vector<string> &v)
{
    int n = v.size();

    if (n > 3 || n == 1)
    {
        printf("argv error\n");
    }
    else
    {
        zftp_message::Pakcet packet;
        zftp_message::pull_msg pull;
        pull.set_name(v[1]);
        pull.set_src_dir(".");

        packet.set_msgid(int(zlkMsg::pull));
        if (n == 2)
        {
            pull.set_dstname(v[1]);
        }
        else
        {
            struct stat st;
            if (stat(v[2].data(), &st) != 0)
            {
                int idx = v[2].find_last_of('/');
                if (idx != -1)
                {
                    pull.set_src_dir(v[2].substr(0, idx));

                    if (idx == v[2].size() - 1)
                    {
                        pull.set_dstname(v[1]);
                    }
                    else
                    {
                        pull.set_dstname(v[2].substr(idx + 1));
                    }
                }
                else
                {
                    pull.set_dstname(v[2]);
                }
            }
            else
            {
                if (st.st_mode & S_IFDIR)
                {
                    pull.set_src_dir(v[2]);
                    pull.set_dstname(v[1]);
                }
                else
                {
                    int idx = v[2].find_last_of('/');
                    if (idx != -1)
                    {
                        pull.set_src_dir(v[2].substr(0, idx));
                        if (idx == v[2].size() - 1)
                        {
                            pull.set_dstname(v[1]);
                        }
                        else
                        {
                            pull.set_dstname(v[2].substr(idx + 1));
                        }
                    }
                    else
                    {
                        pull.set_dstname(v[2]);
                    }
                }
            }
        }
        packet.set_msgbody(pull.SerializeAsString());
        g_client->send_msg(packet);
    }
}
void cmd_cd(vector<string> &v)
{
    zftp_message::Pakcet packet;
    zftp_message::cd_msg pull;
    packet.set_msgid(int(zlkMsg::cd));
    int n = v.size();

    if (n > 1)
    {
        pull.set_dir(v[1]);
    }

    packet.set_msgbody(pull.SerializeAsString());
    g_client->send_msg(packet);
}
void cmd_pwd(vector<string> &v)
{
    zftp_message::Pakcet packet;
    packet.set_msgid(int(zlkMsg::pwd));

    g_client->send_msg(packet);
}
void cmd_push(vector<string> &v)
{
    int n = v.size();
    if (n > 3 || n == 1)
    {
        printf("argv error\n");
    }
    else
    {

#ifndef WIN32
        int b = access(v[1].data(), F_OK);
#else
        int b _access(v[1].data(), 0)
#endif
        if (b == 0)
        {
            zftp_message::Pakcet packet;
            zftp_message::push_msg push;
            packet.set_msgid(int(zlkMsg::push));
            ifstream ifs(v[1], ios::binary);

            ifs.seekg(0, ifs.end);
            auto fos = ifs.tellg();
            ifs.seekg(0, ifs.beg);
            char *pbuf = new char[fos];
            if (pbuf != nullptr)
            {
                ifs.read(pbuf, fos);
                push.set_data(pbuf, fos);
                if (n == 2)
                {
                    push.set_dst_name(v[1]);
                }
                else
                {
                    if (*v[2].rbegin() == '/')
                        v[2] += '/' + v[1];
                    push.set_dst_name(v[2]);
                }

                push.set_src_name(v[1]);
                delete[] pbuf;
            }
            else
            {
            }
            packet.set_msgbody(push.SerializeAsString());
            g_client->send_msg(packet);

            ifs.close();
        }
    }
}
void cmd_mkdir(vector<string> &v)
{
    int n = v.size();

    if (n == 1)
        return;
    zftp_message::Pakcet packet;
    zftp_message::mkdir_msg mk;
    mk.set_data(v[1]);
    packet.set_msgid(int(zlkMsg::mkdir));
    packet.set_msgbody(mk.SerializeAsString());
    g_client->send_msg(packet);
}
void cmd_login(vector<string> &v)
{
    int n = v.size();

    if (n != 3)
    {
        printf("need user and passwd\n");
        return;
    }
    zftp_message::Pakcet packet;
    packet.set_msgid(int(zlkMsg::login));
    zftp_message::login_msg msg;
    msg.set_user(v[1]);
    msg.set_passwd(v[2]);
    packet.set_msgbody(msg.SerializeAsString());

    g_client->send_msg(packet);
}
void cmd_enter(string cmd)
{
    auto v = sp(cmd);
    int n = v.size();
    if (v.size() == 0)
        return;
    string &type = v[0];
    auto iter = pf.find(type);
    if (iter != pf.end())
    {
        iter->second(v);
    }
}

void cmd_ctrl()
{
    pf["ls"] = cmd_ls;
    pf["rm"] = cmd_rm;
    pf["pull"] = cmd_pull;
    pf["mkdir"] = cmd_mkdir;
    pf["cd"] = cmd_cd;
    pf["pwd"] = cmd_pwd;
    pf["push"] = cmd_push;

    zlk_bask bash(cmd_enter);
    bash.regist("pull");
    bash.regist("ls");
    bash.regist("mkdir");
    bash.regist("push");
    bash.regist("pwd");
    bash.regist("cd");
    bash.regist("rm");
    bash.run();
}

int main(int argc, char *argv[])
{
    std::unordered_map<string, string> config;
    printf("%s\n", argv[1]);
    std::string dir = argc == 1 ? "." + sep : argv[1];
    string path = dir + sep + "config.lua";
    printf("%s\n", path.data());

    table_traverse(path, config);
    for (auto x : config)
    {
        cout << x.first << ":" << x.second;
    }

    zlk_log::getInstance()
        .init(config["log"] + sep + "log", 20480, 1);
    io_service_pool isp(2);

    g_client = new Client(isp);
    g_client->regist();
    zlk_boost_socket s(isp);
    s.connect(config["ip"], atoi(config["port"].data()));
    isp.run(false);
    cmd_ctrl();
    printf("%s\n", strerror(errno));
    return 0;
}