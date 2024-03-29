#include <iostream>
#include "lua_load.h"
#include "zlk_ftp_server.h"
#include "boost_socket.h"
#include "zftp_server_handmessage.h"

Server *g_server;
void Server::regist()
{
    regist(new zftp_server_handleMessage);
}
// 注册消息
void Server::regist(zlk_messagehandle *pz)
{
    for (auto x : pz->m_mapFunc)
    {
        m_mapFunc[x.first] = x.second;
        DBG("regist %d", int(x.first));
    }
}

void Server::handle_message(zftp_message::Pakcet &packet, zftp_message::Pakcet &packetResponse)
{

    auto iter = m_mapFunc.find(zlkMsg(packet.msgid()));
    if (iter != m_mapFunc.end())
    {
        DBG("run func msgid = %d", packet.msgid());
        iter->second(packet, packetResponse);
        if (packetResponse.msgbody().size())
        {
            packetResponse.set_msgid(packet.msgid());
        }
    }
    else
    {
        ERR("not find msg id = %d", packet.msgid());
    }
}
int main(int argc, char *argv[])
{
    std::string path = "log/server";
    // if (argc == 2)
    //     path = argv[1];

    printf("adas\n");
    zlk_log::getInstance().init(path, 20480, 1, zlk_logmode_debug);
    usleep(1000);
    printf("adas\n");
    // return 1;
    io_service_pool isp(2);

    g_server = new Server(isp);
    g_server->regist();
    zlk_boost_socket s(isp);
    if (argc == 3)
    {
        s.bind(atoi(argv[2]));
    }
    else
    {
        s.bind();
    }

    s.accept();

    isp.run();

    return 0;
}