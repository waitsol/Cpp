#include "zlk_zftp_client_connect.h"
#include "zlk_ftp_client.h"
extern Client *g_client;

void zlk_zftp_client_connect::hand_message(char *p, int sz)
{
    zftp_message::Pakcet packet, response;
    bool ok = packet.ParseFromArray(p, sz);
    delete[] p;
    if (!ok)
    {

        ERR("ParseFromArray error sz = %d", sz);
        return;
    }
    g_client->handle_message(packet, response);
    if (response.msgbody().size())
    {
        auto res = response.SerializeAsString();
        send_message(res.data(), res.length());
    }
}