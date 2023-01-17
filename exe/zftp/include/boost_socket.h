#ifndef __BOOST_SOCKET_
#define __BOOST_SOCKET_
#include <map>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include "io_service_pool.h"
#include "zlk_log.h"
#include "zlk_ftp_server.h"
#include "zlk_ftp_client.h"
#include "zftp_client_handmessage.h"

using boost::asio::io_service;
using boost::asio::ip::tcp;
using std::map;
extern Server *g_server;
extern Client *g_client;

class zlk_boost_socket
{
private:
    using socket_type = tcp::socket;
    using socket_prt = boost::shared_ptr<socket_type>;
    using enpoint_type = tcp::endpoint;
    using acceptor_type = tcp::acceptor;

public:
    zlk_boost_socket(io_service_pool &iop) : m_io_service_pool(iop)
    {
    }
    void bind(int port = 9999)
    {
        m_acceptor = new acceptor_type(m_io_service_pool.get_acceptor_io_service(), enpoint_type(tcp::v4(), port));
    }
    void accept()
    {
        boost::shared_ptr<tcp::socket> sock(new tcp::socket(m_io_service_pool.get_io_service()));
        m_acceptor->set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
        m_acceptor->async_accept(*sock, boost::bind(&zlk_boost_socket::handle_accept, this, boost::asio::placeholders::error, sock));
    }
    void connect(string ip = "127.0.0.1", int port = 9999)
    {
        m_socket = socket_prt(new socket_type(m_io_service_pool.get_acceptor_io_service()));
        m_socket->async_connect(enpoint_type(boost::asio::ip::address::from_string(ip), port), boost::bind(&zlk_boost_socket::handle_connect, this, boost::asio::placeholders::error, m_socket));
    }

private:
    void handle_accept(const boost::system::error_code &ec, boost::shared_ptr<tcp::socket> sock)
    {
        accept();
        if (ec)
        {
            ERR("error %s", ec.message());
            return;
        }

        DBG("new connect ip = %s  port = %d\n", sock->remote_endpoint().address().to_string().data(), sock->remote_endpoint().port());
        m_io_service_pool.get_io_service().post(boost::bind(&Server::post, g_server, sock));
    }
    void handle_connect(const boost::system::error_code &ec, boost::shared_ptr<tcp::socket> sock)
    {

        if (ec)
        {
            ERR("error %s", ec.message());
            return;
        }

        INF("new connect ip = %s  port = %d\n", sock->remote_endpoint().address().to_string().data(), sock->remote_endpoint().port());
        m_io_service_pool.get_acceptor_io_service().post(boost::bind(&Client::post, g_client, sock));
    }

private:
    io_service_pool &m_io_service_pool;
    acceptor_type *m_acceptor;
    socket_prt m_socket;
};

#endif