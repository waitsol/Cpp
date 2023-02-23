#include "zlk_log.h"
#include <stdarg.h>
#include <unistd.h>
#include <functional>
static const char *modeStr[zlk_logmode_end] = {
    "[zlk_logmode_debug]:",
    "[zlk_logmode_info]:",
    "[zlk_logmode_err]:",

};
zlk_log zlk_log::m_instance;
static const int modeLen[zlk_logmode_end] = {
    20, 19, 18};

zlk_log &zlk_log::getInstance()
{
    return m_instance;
}
void zlk_log::init(string fileName, int buffsize, bool multithread, zlk_logmode print_level)
{
    m_time = 0;
    m_fileName_pre = fileName;
    m_buffsize = buffsize;
    m_multithread = multithread;
    m_running = false;
    // 直接往屏幕输出
    m_print_level = print_level;
    if (fileName.size() == 0)
        return;

    open();
    if (!m_file.is_open())
    {
        throw fileName + " open filed";
    }
    m_running = true;
    int rfd = m_pipe.init();
    if (rfd == -1)
        return;

    m_thread = new thread(std::bind(&zlk_log::run, this, rfd));

    printf("logfile  init success \n");
}
void zlk_log::write(zlk_logmode mode, const char *pszFormat, ...)
{
    //  printf("write\n");

    if ((mode & (zlk_logmode_end - 1)) < m_print_level)
        return;

    static const int buflen = 4096;
    int cn = 0;
    int n = 0;
    char buf[buflen] = {0};
    // cout << pszFormat << endl;
    // pprof测试 这个时间是最耗时的  卡在了 tzset函数那里
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
    if (m_fileName_pre.size() == 0)
    {
        printf("%s", buf);
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
    printf("log thread run\n");
    while (m_running)
    {

        FD_SET(rfd, &rdfds);
        //    printf("selcet\n");
        tv.tv_sec = 0;
        tv.tv_usec = 1000;                                   // 注意单位是微秒
                                                             //   printf("?\n");
        int iRet = select(rfd + 1, &rdfds, NULL, NULL, &tv); // 注意注意
                                                             // printf("??\n");

        if (iRet < 0)
        {
            // 系统中断
            if (errno == 4)
                continue;
            printf("selcet error, iRet %d error = %s %d\n", iRet, strerror(errno), errno);
            m_running = false;
            close(rfd);

            return;
        }
        // cout<<GetTickCount64()<<endl;

        open();
        if (0 == iRet)
        {

            zlk_buffer *pbuf = nullptr;

            // 只可能第一个有数据
            if (m_logBufferList.size())
            {
                lock(m_logMutex);
                // 必须重新判断 如果突然写线程把m_logBufferList强制推送到m_fileBufferList
                if (m_logBufferList.size() && (*m_logBufferList.begin())->leave() != m_buffsize)
                {
                    pbuf = m_logBufferList.front();

                    m_logBufferList.pop_front();
                }
                unlock(m_logMutex);
            }
            // cout << "time out" << (pbuf == nullptr) << endl;

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

            test_time = GetTickCount64();
        }
        unlock(m_logMutex);
        m_file.flush();
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
        snprintf(buf, BUFSIZ, "%s_%d_%d_%02d_%02d_%02d_%02d.log", m_fileName_pre.c_str(), t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
        if (m_file.is_open())
            m_file.close();
        m_file.open(buf);
    }
}
