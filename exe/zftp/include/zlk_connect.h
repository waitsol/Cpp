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
#include <boost/thread/mutex.hpp>
#include <queue>
#include "zlk_log.h"
#define _10MB 1024 * 1024 * 10
class zlk_connect
{
public:
    zlk_connect(int uid, boost::shared_ptr<boost::asio::ip::tcp::socket> sock) : _uid(uid), m_sock(sock)
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

        int end = *(int *)header_buffer_.c_array();

        DBG("header sz = %d\n", end);
        if (end <= 0)
        {
            ERR("read_body error end  = %d", end);
            read_header();
            return;
        }

        m_sock->async_receive(boost::asio::buffer(buffer_.data(), end), boost::bind(&zlk_connect::handle_body,
                                                                                    this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred, 0, end));
    }
    void handle_body(const boost::system::error_code &ec, std::size_t bytes_transferred, int offset, int end)
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
        offset += bytes_transferred;
        if (offset >= end)
        {
            DBG("Read over sz = %d", offset);
            char *msg = new char[end];
            memcpy(msg, buffer_.data(), end);

            // 调用虚函数 不同的服务连接处理不一样
            hand_message(msg, end);
            int d = offset - end;
            if (d >= 4)
            {
                int tmp = end;
                end = *(int *)(header_buffer_.c_array() + tmp);
                if (end <= 0)
                {
                    ERR("handle_body error end  = %d", end);
                    read_header();
                    return;
                }
                offset = d - 4;

                memcpy(header_buffer_.c_array(), header_buffer_.c_array() + tmp, offset);
                m_sock->async_receive(boost::asio::buffer(buffer_.data() + offset, end - offset), boost::bind(&zlk_connect::handle_body,
                                                                                                              this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred, offset, end));
            }
            else
            {

                read_header();
            }
        }
        else
        {
            m_sock->async_receive(boost::asio::buffer(buffer_.data() + offset, end - offset), boost::bind(&zlk_connect::handle_body,
                                                                                                          this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred, offset, end));
        }
    }

public:
    //
    int send_message(const char *msg, int sz)
    {

        boost::lock_guard<boost::mutex> _g(_mu);

        if (_ing == nullptr)
        {
            _ing = new char[sz + 4];
            int_to_array(sz, _ing);
            memcpy(_ing + 4, msg, sz);
            // 4是包头
            m_sock->async_send(boost::asio::buffer(_ing, sz + 4), boost::bind(&zlk_connect::send_message_callback,
                                                                              this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred, 0, sz + 4));
        }
        else
        {
            socket_msg sm;
            sm.data = new char[sz + 4];
            int_to_array(sz, sm.data);
            memcpy(sm.data + 4, msg, sz);
            sm.sz = sz + 4;
            _q_msg.push(sm);
        }
        return 1;
    }

private:
    void send_message_callback(const boost::system::error_code &ec, std::size_t bytes_transferred, int offset, int end)
    {
        if (ec)
        {
            ERR("send_message failed err = %s what = %s", ec.message().data(), ec.what().data());
            return;
        }
        if (offset == 0)
        {
            DBG("send first pack success bytes_transferred = %d,end=%d ", bytes_transferred, end);
        }
        // cao不直接算后面+全是坑
        offset += bytes_transferred;
        if (offset < end)
        {
            m_sock->async_send(boost::asio::buffer(_ing + offset, end - offset), boost::bind(&zlk_connect::send_message_callback,
                                                                                             this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred, offset, end));
        }
        else
        {
            DBG("send over sz = %d", offset);
            boost::lock_guard<boost::mutex> _g(_mu);
            delete[] _ing;
            _ing = nullptr;
            if (_q_msg.size())
            {
                socket_msg sm = _q_msg.front();
                _q_msg.pop();
                _ing = sm.data;
                int sz = sm.sz;
                // 4是包头
                m_sock->async_send(boost::asio::buffer(_ing, sz), boost::bind(&zlk_connect::send_message_callback,
                                                                              this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred, 0, sz));
            }
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
    struct socket_msg
    {
        char *data;
        int sz;
    };

private:
    boost::shared_ptr<boost::asio::ip::tcp::socket> m_sock;
    boost::array<char, _10MB> buffer_;
    boost::array<char, 4> header_buffer_;
    boost::mutex _mu;
    std::queue<socket_msg> _q_msg;
    char *_ing;
};

#endif