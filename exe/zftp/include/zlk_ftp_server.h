#ifndef __ZLK_FTP_SERVER
#define __ZLK_FTP_SERVER
#include <iostream>
#include <boost/asio.hpp>
#include "zlk_zftp_server_connect.h"
#include "zlk_message.pb.h"
#include "zlk_messagehandle.h"
#include <list>
#include <string>
#include "io_service_pool.h"
struct UserInfo
{
    std::list<std::string> _list;
    std::map<int, std::string> _mapFileInfo;
};
class Server
{
public:
    Server(io_service_pool &isp) : m_io_service_pool(isp) {}
    void post(boost::shared_ptr<boost::asio::ip::tcp::socket> sock)
    {
        m_connectInfo[m_idx] = boost::shared_ptr<zlk_zftp_server_connect>(new zlk_zftp_server_connect(m_idx, sock));
        _mapUserInfo[m_idx] = UserInfo();

        m_connectInfo[m_idx++]->read_header();
    }
    io_service_pool &get_io_service_pool()
    {
        return m_io_service_pool;
    }
    void handle_message(zftp_message::Pakcet &packet, zftp_message::Pakcet &packetResponse);
    void regist();
    void logout(int id)
    {
        m_connectInfo.erase(id);
        _mapUserInfo.erase(id);
    }
    UserInfo *get_userinfo(int id)
    {
        auto iter = _mapUserInfo.find(id);
        if (iter != _mapUserInfo.end())
        {
            return &iter->second;
        }
        return nullptr;
    }

private:
    void regist(zlk_messagehandle *);

private:
    int m_idx = 1;
    map<int64_t, boost::shared_ptr<zlk_zftp_server_connect>> m_connectInfo;
    std::map<zlkMsg, zlk_msg_func> m_mapFunc;
    std::map<int, UserInfo> _mapUserInfo;
    io_service_pool &m_io_service_pool;
};
#endif