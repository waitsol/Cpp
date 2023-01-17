#include <iostream>
#include "lua_load.h"
#include "zlk_ftp_server.h"
#include "boost_socket.h"
#include "zlk_message.pb.h"
#include "zftp_handmessage.h"

Server *g_server;
void Server::regist();
{
    regist(new zftp_handleMessage);
}
// 注册消息
void Server::regist(zlk_messagehandle *pz)
{
    for (auto x : pz->m_mapFunc)
    {
        m_mapFunc[x->first] = x->second;
    }
}

void Server::handle_message(const char *data, int sz)
{
    zftp_message::Pakcet packet;

    bool ok = packet.ParseFromArray(data, sz);
    if (!ok)
    {
        ERR("ParseFromArray error sz = %d", sz);
        return;
    }
    zftp_message::response_msg res;
    auto iter = m_mapFunc.find(packet.msgid());
    if (iter != m_mapFunc.end())
    {
        (*iter)(packet, res);
    }
    else
    {
        ERR("not find msg id = %d", packet.msgid());
    }
}
int main()
{
    zlk_log::getInstance().init("", 1, 1);
    g_server = new Server();
    io_service_pool isp(2);
    zlk_boost_socket s(isp, 1111);
    isp.run();
    return 0;
}