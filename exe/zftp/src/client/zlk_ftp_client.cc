#include <iostream>
#include "lua_load.h"
#include "zlk_ftp_client.h"
#include "boost_socket.h"
#include "zlk_message.pb.h"
#include "zftp_client_handmessage.h"
#include "zlk_string.h"
#ifdef WIN32
#define sep '\\'
#else
#define sep '/'
#endif
Client *g_client;
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
void Client::send_msg(const zftp_message::Pakcet &packet)
{

    string s;
    if (packet.SerializeToString(&s))
    {
        m_connect->send_message(s.data(), s.size());
    }
    else
    {
        ERR("send_msg error");
    }
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
void cmd_ctrl()
{
    char cmd[2048];
    while (1)
    {

        // 清除缓冲区
        memset(cmd, 0, 2048);
        scanf("%[^\n]", cmd);
        auto v = sp(cmd);
        int n = v.size();
        getchar();
        printf("n= %d cmd = %s\n", n, cmd);
        if (v.size() == 0)
            continue;
        string &type = v[0];

        if (type == "ls")
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
        else if (type == "rm")
        {
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
        else if (type == "pull")
        {
            if (n > 3)
            {
                printf("argv error\n");
            }
            else
            {
                zftp_message::Pakcet packet;
                zftp_message::pull_msg pull;
                pull.set_name(v[1]);
                packet.set_msgid(int(zlkMsg::pull));
                if (n == 2)
                {
                    pull.set_dstname(v[1]);
                }
                else
                {
                    struct stat st;
                    if (stat(v[2].data(), &st) == 0 && (st.st_mode & S_IFDIR))
                    {
                        pull.set_dstname(v[2] + sep + v[1]);
                    }
                    else
                    {
                        pull.set_dstname(v[1]);
                    }
                }

                packet.set_msgbody(pull.SerializeAsString());
                g_client->send_msg(packet);
            }
        }
        else if (type == "cd")
        {
            zftp_message::Pakcet packet;
            zftp_message::cd_msg pull;
            packet.set_msgid(int(zlkMsg::cd));
            if (n > 1)
            {
                pull.set_dir(v[1]);
            }

            packet.set_msgbody(pull.SerializeAsString());
            g_client->send_msg(packet);
        }
        else if (type == "pwd")
        {
            zftp_message::Pakcet packet;
            packet.set_msgid(int(zlkMsg::pwd));

            g_client->send_msg(packet);
        }
        else if (type == "push")
        {
            if (n > 3)
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
                printf("%d\n", b);
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
                        push.set_dst_name(n == 3 ? v[2] : v[1]);
                        push.set_src_name(v[1]);
                        delete[] pbuf;
                    }
                    packet.set_msgbody(push.SerializeAsString());
                    g_client->send_msg(packet);

                    ifs.close();
                }
            }
        }
        else if (type == "mkdir")
        {
            if (n == 1)
                continue;
            zftp_message::Pakcet packet;
            zftp_message::mkdir_msg mk;
            mk.set_data(v[1]);
            packet.set_msgid(int(zlkMsg::mkdir));
            packet.set_msgbody(mk.SerializeAsString());
            g_client->send_msg(packet);
        }
        else if (type == "login")
        {
            if (n != 3)
            {
                printf("need user and passwd\n");
                continue;
            }
            zftp_message::Pakcet packet;
            packet.set_msgid(int(zlkMsg::login));
            zftp_message::login_msg msg;
            msg.set_user(v[1]);
            msg.set_passwd(v[2]);
            packet.set_msgbody(msg.SerializeAsString());

            g_client->send_msg(packet);
        }
        else
        {
            printf("no find %s\n", v[0].data());
        }
    }
}
int main()
{
    zlk_log::getInstance().init("", 1, 1);
    io_service_pool isp(2);

    g_client = new Client(isp);
    g_client->regist();
    zlk_boost_socket s(isp);
    s.connect();
    isp.run(false);
    cmd_ctrl();
    printf("%s\n", strerror(errno));
    return 0;
}