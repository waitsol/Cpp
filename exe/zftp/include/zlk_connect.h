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
    }
    void read_header()
    {
        memset(header_buffer_.data(), 0, header_buffer_.size());
        m_sock->async_receive(boost::asio::buffer(header_buffer_, 4), boost::bind(&zlk_connect::read_body,
                                                                                  this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
    }

protected:
    void read_body(const boost::system::error_code &ec, std::size_t bytes_transferred)
    {
        if (ec)
        {
            ERR("read_body failed = %s", ec.message().data());
            return;
        }
        /*；连接断开bytes_transferred=0*/
        if (bytes_transferred == 0)
        {
            DBG("socket disconnect");
            return;
        }
        if (bytes_transferred != 4)
        {
            ERR("bytes_transferred error = %d", bytes_transferred);
            read_header();
            return;
        }
        int sz = *(int *)header_buffer_.c_array();
        DBG("header sz = %d\n", sz);
        m_sock->async_receive(boost::asio::buffer(buffer_), boost::bind(&zlk_connect::handle_body,
                                                                        this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
    }
    void handle_body(const boost::system::error_code &ec, std::size_t bytes_transferred)
    {
        DBG("bytes_transferred = %d", bytes_transferred);

        if (ec)
        {
            ERR("handle_body failed = %s", ec.message().data());
            return;
        }
        /*；连接断开bytes_transferred=0*/
        if (bytes_transferred == 0)
        {
            ERR("socket disconnect");
            return;
        }
        char *msg = new char[bytes_transferred];
        memcpy(msg, buffer_.data(), bytes_transferred);

        // 调用虚函数 不同的服务连接处理不一样
        hand_message(msg, bytes_transferred);
        read_header();
    }

public:
    //
    int send_message(const char *msg, int sz)
    {
        int_to_array(sz, send_buffer_.c_array());
        memcpy(send_buffer_.c_array() + 4, msg, sz);
        // 4是包头
        m_sock->async_send(boost::asio::buffer(send_buffer_, sz + 4), boost::bind(&zlk_connect::send_message_callback,
                                                                                  this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
        return 1;
    }

private:
    void send_message_callback(const boost::system::error_code &ec, std::size_t bytes_transferred)
    {
        if (ec)
        {
            ERR("send_message failed err = %s", ec.message());
        }
        DBG("send_message sz = %d", bytes_transferred);
    }

protected:
    virtual void hand_message(char *msg, int sz) = 0;

private:
    void int_to_array(int sz, char *p)
    {
        for (int i = 0; i < 4; i++)
        {
            p[i] = (sz & 0xff);
            sz >>= 8;
        }
    }

private:
    boost::shared_ptr<boost::asio::ip::tcp::socket> m_sock;
    boost::array<char, 81900> buffer_;
    boost::array<char, 81900> send_buffer_;
    boost::array<char, 4> header_buffer_;
};

#endif