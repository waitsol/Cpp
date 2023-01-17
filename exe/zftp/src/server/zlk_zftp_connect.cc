#include "zlk_zftp_connect.h"
extern Server *g_server;

class zlk_zftp_connect : public zlk_connect
{
public:
    zlk_zftp_connect(boost::shared_ptr<boost::asio::ip::tcp::socket> sock) : zlk_connect(sock) {}
    void hand_message(char *p, int sz)
    {
        DBG("sz = %d p = %s", sz, p);
        g_server->handle_message(p, sz);
    }
};
