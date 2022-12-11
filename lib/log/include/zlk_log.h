#ifndef __ZLK_LOG_H
#define __ZLK_LOG_H
#include "zlk_buffer.h"
#include "zlk_pipe.h"
#include <list>
#include <fstream>
#include <thread>
#include <string>
#include <mutex>
using namespace std;
inline long long GetTickCount64()
{
    struct timeval tv;
    struct timezone tz;

    gettimeofday(&tv, &tz);
    return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}
enum zlk_logmode:int
{
    zlk_logmode_err,
    zlk_logmode_info,
    zlk_logmode_debug,
    zlk_logmode_end,
    zlk_logmode_line=1<<16,
};

class zlk_log
{
public:
    void init(string fileName, int buffsize, bool multithread=false);
    void write(zlk_logmode mode, const char *pszFormat, ...);
    static zlk_log &getInstance();
    void debug()
    {
        int a=0;
        for (auto x : m_fileBufferList)
        {
            a+=x->getline();
            cout << x->data();
        }
        for (auto x : m_logBufferList)
        {
            a += x->getline();

              cout<<x->data();
        }
        cout<<"a = "<<a<<endl;
    }
    void uninit()
    {
        m_pipe.close();
        m_file.close();
    }
private:
    void run();
    zlk_log() = default;
    void lock(mutex &m)
    {
        if (m_multithread)
            m.lock();
    }
    void unlock(mutex &m)
    {
        if (m_multithread)
            m.unlock();
    }
    void open();
    void sync2file(zlk_buffer *pbuf = nullptr);

private:
    ofstream m_file;
    list<zlk_buffer *> m_logBufferList;
    list<zlk_buffer *> m_fileBufferList;
    zlk_pipe m_pipe;
    mutex m_logMutex;
    mutex m_fileMutex;
    int m_buffsize;
    bool m_running;
    thread *m_thread;
    bool  m_multithread;

    time_t m_time;
    string  m_fileName_pre;
public:
    static zlk_log m_instance;
    int64_t t;
    int64_t m_sz;
    
};
#endif