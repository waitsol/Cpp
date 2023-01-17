#include <iostream>
#include "lua_load.h"
#include "zlk_ftp_client.h"
#include "boost_socket.h"
#include "zlk_message.pb.h"
#include "zftp_client_handmessage.h"

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
void Client::handle_message(zftp_message::Pakcet& packet, zftp_message::Pakcet& packetResponse)
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
    while (scanf("%s", cmd))
    {
        if (cmd[0] == '1')
        {
            zftp_message::Pakcet packet;
            zftp_message::ls_msg ls;
            ls.set_data("include");
            packet.set_msgid(int(zlkMsg::ls));
            string str;
            ls.SerializeToString(&str);
            packet.set_msgbody(str);
            g_client->send_msg(packet);
        }
        else
        {
            DBG(cmd);
        }
        memset(cmd, 0, 2048);
    }
}
int main()
{
    zlk_log::getInstance().init("", 1, 1);
    g_client = new Client();
    g_client->regist();
    io_service_pool isp(2);
    zlk_boost_socket s(isp);
    s.connect();
    isp.get_io_service().post(std::bind(cmd_ctrl));
    isp.run();
    return 0;
}