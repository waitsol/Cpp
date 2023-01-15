#include "zlk_log.h"
#include <stdarg.h>
#include <unistd.h>
#include <functional>
static const char *modeStr[zlk_logmode_end] = {
    "[zlk_logmode_err]:",
    "[zlk_logmode_info]:",
    "[zlk_logmode_debug]:",

};
zlk_log zlk_log::m_instance;
static const int modeLen[zlk_logmode_end] = {
    18, 29, 20};

zlk_log &zlk_log::getInstance()
{
    return m_instance;
}
void zlk_log::init(string fileName, int buffsize, bool multithread)
{
    m_time = 0;
    m_fileName_pre = fileName;
    m_buffsize = buffsize;
    m_multithread = multithread;
    m_running = false;
    //直接往屏幕输出
    if(fileName.size()==0)
        return;

    open();
    if (!m_file.is_open())
    {
        throw fileName + " open filed";
    }
    m_running = true;
    int rfd = m_pipe.init();
    if(rfd==-1)
        return;

    m_thread = new thread(std::bind(&zlk_log::run, this,rfd));
    
    printf("log init success\n");
}
void zlk_log::write(zlk_logmode mode, const char *pszFormat, ...)
{
    static const int buflen = 4096;
    int cn = 0;
    int n = 0;
    char buf[buflen] = {0};
    // cout << pszFormat << endl;
    struct timeval now;
    gettimeofday(&now, nullptr);
    tm *t = localtime(&now.tv_sec);
    cn = snprintf(buf, buflen, "[%d:%d:%d:%ld]", t->tm_hour, t->tm_min, t->tm_sec, now.tv_usec / 1000);

    va_list args;
    va_start(args, pszFormat);
    if (mode & zlk_logmode_line)
    {
        mode = zlk_logmode(int(mode) ^ int(zlk_logmode_line));
        n = vsnprintf(buf + cn, buflen - cn, "[FILE = %s,Func = %s,Line = %d]", args);
        cn += n;
        //    cout<<cn<<endl;
    }
    //   cout << buf << endl;
    memcpy(buf + cn, modeStr[mode], modeLen[mode]);
    cn += modeLen[mode];
    cn += vsnprintf(buf + cn, buflen - cn, pszFormat, args);
    va_end(args);
    // cout << cn << endl;
    buf[cn] = '\n';
    if(m_fileName_pre.size()==0)
    {
        printf("%s",buf);
        return;
    }
    cn += 1;
    {
        // cout << buf << endl;
        lock(m_logMutex);
        zlk_buffer *pbuf = nullptr;

        while (m_logBufferList.size())
        {
            pbuf = m_logBufferList.front();
            m_logBufferList.pop_front();
            if (pbuf->leave() >= cn)
            {
                break;
            }
            else
            {
                // 第二次按道理就必定不会出现这种情况  所以这里加锁
                lock(m_fileMutex);
                m_fileBufferList.push_back(pbuf);
                pbuf = nullptr;
                unlock(m_fileMutex);
                m_pipe.notify();
            }
        }
        if (!pbuf)
        {
            // cout<<"new "<<endl;
            pbuf = new zlk_buffer(m_buffsize);
            if (pbuf == nullptr)
            {
                cerr << "pbuf = nullptr" << endl;
            }
        }
        if (pbuf)
        {
            pbuf->append(buf, cn);
            m_logBufferList.push_front(pbuf);
        }

        unlock(m_logMutex);
    }
}
void zlk_log::run(int rfd)
{
    //  cout << "rfd =" << rfd << endl;
    struct timeval tv; // 超时时间
    tv.tv_sec = 0;
    tv.tv_usec = 1000; // 注意单位是微秒

    fd_set rdfds;
    FD_ZERO(&rdfds);              // 描述集初始化
    FD_SET(STDIN_FILENO, &rdfds); // STDIN_FILENO是标准输入， 塞入描述集

    char buf[BUFSIZ];

    list<zlk_buffer *> blist;
    while (m_running)
    {

        FD_SET(rfd, &rdfds);
        //    printf("selcet\n");
        int iRet = select(rfd + 1, &rdfds, NULL, NULL, &tv); // 注意注意
        if (iRet < 0)
        {
            printf("selcet error, iRet %d\n", iRet);
            m_running = false;
            close(rfd);

            return;
        }
        // cout<<GetTickCount64()<<endl;
        tv.tv_sec = 0;
        tv.tv_usec = 1000; // 注意单位是微秒
        t = 0;
        open();
        if (0 == iRet)
        {

            t = GetTickCount64();
            //  cout<<"time out"<<endl;
            zlk_buffer *pbuf = nullptr;

            // 只可能第一个有数据
            if (m_logBufferList.size())
            {
                lock(m_logMutex);
                pbuf = m_logBufferList.front();
                m_logBufferList.pop_front();
                unlock(m_logMutex);
            }
            sync2file(pbuf);
        }
        else
        {
            if (read(rfd, buf, BUFSIZ) <= 0)
            {
                m_running = false;
                close(rfd);
            }
            else
            {

                sync2file();
            }
        }
    }
}
void zlk_log::sync2file(zlk_buffer *pbuf)
{
    list<zlk_buffer *> blist;

    {
        // 做一个缓冲 这时候 write可以将buffer添加到m_fileBufferList
        lock(m_fileMutex);
        while (m_fileBufferList.size())
        {
            blist.push_back(m_fileBufferList.front());
            m_fileBufferList.pop_front();
        }
        unlock(m_fileMutex);
    }
    if (pbuf)
        blist.push_back(pbuf);
    for (auto it = blist.begin(); it != blist.end(); it++)
    {
        pbuf = *it;
        if (pbuf)
        {
            m_sz += pbuf->write2File(m_file);
            pbuf->reset();
        }
    }

    {

        lock(m_logMutex);

        while (blist.size())
        {
            //   cout << m_logBufferList.size() << endl;
            if (m_logBufferList.size() < 10)
            {
                m_logBufferList.push_back(blist.front());
            }
            else
            {
                // cout<<"delete"<<endl;
                delete blist.front();
            }

            blist.pop_front();
        }
        unlock(m_logMutex);
    }
}
void zlk_log::open()
{

    time_t now = time(0);
    if (now - m_time >= 60 * 60 || m_sz > 1024 * 1024 * 500)
    {
        m_sz = 0;
        tm *t = localtime(&now);
        m_time = now;
        char buf[BUFSIZ] = {0};
        snprintf(buf, BUFSIZ, "%s_%d_%d_%d_%d_%d_%d.log", m_fileName_pre.c_str(), t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
        if (m_file.is_open())
            m_file.close();
        m_file.open(buf);
    }
}
