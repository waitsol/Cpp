#ifndef __ZLK_ZFTP_CONNECT
#define __ZLK_ZFTP_CONNECT
#include "zlk_connect.h"
#include "zlk_ftp_server.h"
class zlk_zftp_connect : public zlk_connect
{
public:
    zlk_zftp_connect(boost::shared_ptr<boost::asio::ip::tcp::socket> sock) : zlk_connect(sock) {}
    void hand_message(char *p, int sz) override;
};
#endif