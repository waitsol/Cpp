#ifndef __ZLK_FTP_SERVER
#define __ZLK_FTP_SERVER
#include <iostream>
#include <boost/asio.hpp>
#include "zlk_zftp_connect.h"
#include "zlk_messagehandle.h"
class Server
{
public:
    void post(boost::shared_ptr<boost::asio::ip::tcp::socket> sock)
    {
        m_connectInfo[m_idx] = boost::shared_ptr<zlk_zftp_connect>(new zlk_zftp_connect(sock));
        m_connectInfo[m_idx++]->read_header();
    }
    void handle_message(const char *data, int sz);
    void regist();

private:
    void regist(zlk_messagehandle *);

private:
    int m_idx = 0;
    map<int64_t, boost::shared_ptr<zlk_zftp_connect>> m_connectInfo;
    std::map<zlkMsg, zlk_msg_func> m_mapFunc;
};
#endif