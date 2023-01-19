#ifndef __zlk_zftp_server_connect
#define __zlk_zftp_server_connect
#include "zlk_connect.h"

class zlk_zftp_server_connect : public zlk_connect
{
public:
    zlk_zftp_server_connect(int uid, boost::shared_ptr<boost::asio::ip::tcp::socket> sock) : zlk_connect(uid, sock), _login(false) {}
    void hand_message(char *p, int sz) override;
    void set_login() { _login = true; };

private:
    bool is_login() { return _login; };
    void _hand_message(char *p, int sz);

private:
    bool _login;
};
#endif