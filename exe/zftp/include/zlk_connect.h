#ifndef __ZLK_CONNECT_H
#define __ZLK_CONNECT_H
#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <boost/thread.hpp>
#include <boost/any.hpp>
#include <boost/date_time.hpp>
#include <boost/chrono.hpp>
#include <boost/thread.hpp>
#include "zlk_log.h"
class zlk_connect
{
public:
    zlk_connect(boost::shared_ptr<boost::asio::ip::tcp::socket> sock) : m_sock(sock)
    {
        m_sock->async_read_some(boost::asio::buffer(buffer_), boost::bind(&zlk_connect::handle_recv,
                                                                          this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
    }
    void read();
    void handle_recv(const boost::system::error_code &ec, std::size_t bytes_transferred)
    {
        std::cout << "handle_recv" << std::endl;
        if (ec)
        {
            std::cout << "handle_recv failed " << ec.message() << std::endl;
            return;
        }
        std::cout << boost::this_thread::get_id() << "handle_recv " << &buf[0] << "  bytes_transferred=" << bytes_transferred << std::endl;
        /*；连接断开bytes_transferred=0*/
        if (bytes_transferred == 0)
        {
            INF("socket disconnect");
        }
        std::string s(buf.begin(), buf.begin() + bytes_transferred);
        char *msg = new char[bytes_transferred];
        memcpy(msg, buffer_.data(), bytes_transferred);
        hand_message(msg, bytes_transferred);
        read();
    }

protected:
    virtual void hand_message(char *msg, int sz) = 0;

private:
    boost::shared_ptr<boost::asio::ip::tcp::socket> m_sock;
    boost::array<char, 81920> buffer_;
};

#endif