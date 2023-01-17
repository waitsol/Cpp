#ifndef __ZLK_FTP_CLIENT
#define __ZLK_FTP_CLIENT
#include <iostream>
#include <boost/asio.hpp>
#include "zlk_messagehandle.h"
#include "zlk_zftp_client_connect.h"
#include "zlk_message.pb.h"
class Client
{
public:
    void post(boost::shared_ptr<boost::asio::ip::tcp::socket> sock)
    {
        m_connect = boost::shared_ptr<zlk_zftp_client_connect>(new zlk_zftp_client_connect(sock));
        m_connect->read_header();
    }
    void handle_message(zftp_message::Pakcet& packet, zftp_message::Pakcet &packetResponse);
    void regist();
    void send_msg(const zftp_message::Pakcet &packet);

private:
    void regist(zlk_messagehandle *);

private:
    boost::shared_ptr<zlk_zftp_client_connect> m_connect;
    std::map<zlkMsg, zlk_msg_func> m_mapFunc;
};
#endif