#ifndef __ZLK_ZFTP_CONNECT
#define __ZLK_ZFTP_CONNECT
#include "zlk_connect.h"

class zlk_zftp_client_connect : public zlk_connect
{
public:
    zlk_zftp_client_connect(boost::shared_ptr<boost::asio::ip::tcp::socket> sock) : zlk_connect(sock) {}
    void hand_message(char *p, int sz) override;
};
#endif