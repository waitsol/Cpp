#include "zlk_zftp_server_connect.h"
#include "zlk_ftp_server.h"
extern Server *g_server;

void zlk_zftp_server_connect::hand_message(char *p, int sz)
{

    zftp_message::Pakcet packet, response;
    bool ok = packet.ParseFromArray(p, sz);
    delete[] p;
    if (!ok)
    {

        ERR("ParseFromArray error sz = %d", sz);
        return;
    }
    g_server->handle_message(packet, response);
    if (response.msgbody().size())
    {
        auto res = response.SerializeAsString();
        send_message(res.data(), res.size());
        DBG("respon sz = %d", res.length());
    }
    else
    {
        DBG("response.msgbody().size() = %d", response.msgbody().size());
    }
}