#include "zlk_zftp_client_connect.h"
#include "zlk_ftp_client.h"
extern Client *g_client;

void zlk_zftp_client_connect::_hand_message(char *p, int sz)
{
    zftp_message::Pakcet packet, response;
    bool ok = packet.ParseFromArray(p, sz);
    delete[] p;
    if (!ok)
    {

        ERR("ParseFromArray error sz = %d", sz);
        return;
    }
    if (packet.msgid() != int(zlkMsg::login))
    {
        g_client->handle_message(packet, response);
        if (response.msgbody().size())
        {
            auto res = response.SerializeAsString();
            send_message(res.data(), res.length());
        }
    }
    else
    {
        zftp_message::response_msg res;

        if (res.ParseFromString(packet.msgbody()))
        {
            if (res.ok())
            {
                printf("login success\n");
            }
            else
            {
                printf("%s\n", res.error().data());
            }
        }
        else
        {
            printf("login stats error\n");
        }
    }
}
void zlk_zftp_client_connect::hand_message(char *p, int sz)
{
    g_client->get_io_service_pool().get_io_service().post(std::bind(&zlk_zftp_client_connect::_hand_message, this, p, sz));
}