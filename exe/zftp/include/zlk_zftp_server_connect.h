#ifndef __zlk_zftp_server_connect
#define __zlk_zftp_server_connect
#include "zlk_connect.h"

class zlk_zftp_server_connect : public zlk_connect
{
public:
    zlk_zftp_server_connect(boost::shared_ptr<boost::asio::ip::tcp::socket> sock) : zlk_connect(sock) {}
    void hand_message(char *p, int sz) override;
};
#endif