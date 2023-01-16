#ifndef __BOOST_SOCKET_
#define __BOOST_SOCKET_
#include <map>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include "io_service_pool.h"
using boost::asio::io_service;
using boost::asio::ip::tcp;
using std::map;
class zlk_boost_socket
{
private:
    using socket_type = tcp::socket;
    using socket_prt = boost::shared_ptr<socket_type>;
    using enpoint_type = tcp::endpoint;
    using acceptor_type = tcp::acceptor;

public:
    zlk_boost_socket(io_service_pool &iop, int port = 1124) : m_io_service_pool(iop), m_acceptor(m_io_service_pool.get_acceptor_io_service(), enpoint_type(tcp::v4(), port))
    {
        
    }

private:
    io_service_pool &m_io_service_pool;
    acceptor_type m_acceptor;
};

#endif