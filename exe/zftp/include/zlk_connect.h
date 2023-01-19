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
#define _10MB 1024 * 1024 * 10
class zlk_connect
{
public:
    zlk_connect(int uid, boost::shared_ptr<boost::asio::ip::tcp::socket> sock) : _uid(uid), m_sock(sock)
    {
        _end = 0;
    }
    void read_header()
    {
        _end = _offset = 0;
        memset(header_buffer_.data(), 0, header_buffer_.size());
        m_sock->async_receive(boost::asio::buffer(header_buffer_, 4), boost::bind(&zlk_connect::read_body,
                                                                                  this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
    }

protected:
    void read_body(const boost::system::error_code &ec, std::size_t bytes_transferred)
    {
        if (ec)
        {
            ERR("read_body failed = %s what = %s  ret = %d", ec.message().data(), ec.what().data(), ec.value());
            close();
            return;
        }
        /*；连接断开bytes_transferred=0*/
        if (bytes_transferred == 0)
        {
            DBG("socket disconnect");
            close();
            return;
        }
        if (bytes_transferred != 4)
        {
            ERR("bytes_transferred error = %d", bytes_transferred);
            read_header();
            return;
        }

        _end = *(int *)header_buffer_.c_array();
        _offset = 0;
        DBG("header sz = %d\n", _end);

        m_sock->async_receive(boost::asio::buffer(buffer_.data() + _offset, _end - _offset), boost::bind(&zlk_connect::handle_body,
                                                                                                         this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
    }
    void handle_body(const boost::system::error_code &ec, std::size_t bytes_transferred)
    {

        if (ec)
        {

            ERR("handle_body failed = %s", ec.message().data());
            close();
            return;
        }
        /*；连接断开bytes_transferred=0*/
        if (bytes_transferred == 0)
        {
            ERR("socket disconnect");
            close();
            return;
        }
        _offset += bytes_transferred;
        DBG("bytes_transferred = %d _offset = %d", bytes_transferred, _offset);

        if (_offset >= _end)
        {
            char *msg = new char[_end];
            memcpy(msg, buffer_.data(), _end);

            // 调用虚函数 不同的服务连接处理不一样
            hand_message(msg, _end);
            int d = _offset - _end;
            if (d >= 4)
            {
                int tmp = _end;
                _end = *(int *)(header_buffer_.c_array() + tmp);
                _offset = d - 4;
                memcpy(header_buffer_.c_array(), header_buffer_.c_array() + tmp, _offset);
                m_sock->async_receive(boost::asio::buffer(buffer_.data() + _offset, _end - _offset), boost::bind(&zlk_connect::handle_body,
                                                                                                                 this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
            }
            else
            {
                read_header();
            }
        }
        else
        {
            m_sock->async_receive(boost::asio::buffer(buffer_.data() + _offset, _end - _offset), boost::bind(&zlk_connect::handle_body,
                                                                                                             this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
        }
    }

public:
    //
    int send_message(const char *msg, int sz)
    {
        int_to_array(sz, send_buffer_.c_array());
        memcpy(send_buffer_.c_array() + 4, msg, sz);
        // 4是包头
        m_sock->async_send(boost::asio::buffer(send_buffer_, sz + 4), boost::bind(&zlk_connect::send_message_callback,
                                                                                  this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred, 0, sz + 4));
        return 1;
    }

private:
    void send_message_callback(const boost::system::error_code &ec, std::size_t bytes_transferred, int offset, int end)
    {
        if (ec)
        {
            ERR("send_message failed err = %s what = %s", ec.message(), ec.what());
            return;
        }
        DBG("send_message sz = %d", bytes_transferred);
        // cao不直接算后面+全是坑
        offset += bytes_transferred;
        if (offset < end)
        {
            m_sock->async_send(boost::asio::buffer(send_buffer_.data() + offset, end - offset), boost::bind(&zlk_connect::send_message_callback,
                                                                                                            this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred, offset, end));
        }
    }

public:
    virtual void hand_message(char *msg, int sz) = 0;
    void shutdown()
    {
        m_sock->close();
    }

protected:
    virtual void close() = 0;

private:
    void int_to_array(int sz, char *p)
    {
        for (int i = 0; i < 4; i++)
        {
            p[i] = (sz & 0xff);
            sz >>= 8;
        }
    }

protected:
    int _uid;

private:
    boost::shared_ptr<boost::asio::ip::tcp::socket> m_sock;
    boost::array<char, _10MB> buffer_;
    boost::array<char, _10MB> send_buffer_;
    boost::array<char, 4> header_buffer_;
    int _offset;
    int _end;
};

#endif