#include "zlk_zftp_server_connect.h"
#include "zlk_ftp_server.h"
extern Server *g_server;
void zlk_zftp_server_connect::_hand_message(char *p, int sz)
{

    zftp_message::Pakcet packet, response;
    bool ok = packet.ParseFromArray(p, sz);
    delete[] p;
    if (!ok)
    {

        ERR("ParseFromArray error sz = %d", sz);
        return;
    }
    packet.set_id(_uid);
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
void zlk_zftp_server_connect::hand_message(char *p, int sz)
{
    g_server->get_io_service_pool().get_io_service().post(std::bind(&zlk_zftp_server_connect::_hand_message, this, p, sz));
}