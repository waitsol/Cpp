/*
    简单的异步样例   pthread_attr_t 参数没弄明白
    test是一个简单的异步写操作  
    tcp是一个简单的echo服务 连接建立之后 输出...  直到对方发消息过来
    只是样例 没有做什么异常处理

*/
#include "../../common/common.h"
#include "../../common/tcpHead.h"
#include <sys/types.h>
#include <aio.h>
#include <pthread.h>
using namespace std;
pthread_attr_t attr = {0};

struct fdbuf{
    int fd;
    void *buf;
};
class Aio{
public:
    void test()
    {
        int err;
        aiocb aio = {0};
        aiocb *aioarr[] = {&aio};
        sigevent &sev = aio.aio_sigevent;
        char buf[1024] = "aio write file\n";

        int fd= open("test.txt",O_WRONLY);
        int confd=-1;
        ERR(fd>=0);

        aio.aio_fildes=fd;
        aio.aio_nbytes=strlen(buf);
        sev.sigev_notify=SIGEV_THREAD;
        sev._sigev_un._sigev_thread._function =&Aio::writecallback;
        aio.aio_buf=buf;
        aio_write(&aio);
        aio_suspend(aioarr,1,nullptr);
        err=aio_error(&aio);
        cout<<"err = "<<err<<endl;
    EXIT:
        return ;
    }
    void tcp()
    {
        int err;
        aiocb aio = {0};
        aiocb *aioarr[] = {&aio};
        sigevent &sev = aio.aio_sigevent;
        char buf[1024] = {0};

        int fd = GetSockBlockFd();
        int confd = -1;
        int k = 0;

        ERR(fd >= 0);

        confd = accept(fd, nullptr, nullptr);
        ERR(confd >= 0);
        fdbuf fb;
        fb.fd=confd;
        fb.buf=buf;
        aio.aio_fildes = confd;
        aio.aio_nbytes = 1024;
        sev.sigev_notify = SIGEV_THREAD;
        sev._sigev_un._sigev_thread._function = &Aio::readcallback;
        sev._sigev_un._sigev_thread._attribute=&attr;
        sev.sigev_value.sival_ptr = (void *)(&fb);
        aio.aio_buf = buf;
        aio_read(&aio);
        while(true)
        {
            sleep(1);
            if(strlen(buf))
            {
                break;
            }else{
                cout<<"."<<flush;
            }
        }
        aio_suspend(aioarr, 1, nullptr);
        err = aio_error(&aio);
        cout << "err = " << err << endl;
        close(confd);
        close(fd);
    EXIT:
        return;
    }

private:
    static void writecallback(sigval sv)
    {
        cout << "\nwrite callback thread id = " << this_thread::get_id() << endl;
        cout << "\nwrite callback thread id = " << pthread_self() << endl;
    }
    static void readcallback(sigval sv)
    {

        cout << "\nread callback thread id = " << this_thread::get_id() << endl;
        cout << "\nread callback thread id = " << pthread_self() << endl;
        fdbuf fb = *(fdbuf *)sv.sival_ptr;
        cout << (char *)fb.buf ;
        write(fb.fd, fb.buf, strlen((char *)fb.buf));

    }
};
void *pthreadF(void *)
{
    while (true)
    {
        sleep(1);
        cout << "|" << flush;
    }
}
int main(int argc,char *argv[])
{
    cout<<this_thread::get_id()<<endl;
    cout << pthread_self() << endl;
    cout << getpid() << endl;

    pthread_attr_init(&attr);
    pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_t pt_1;
    pthread_create(&pt_1, &attr, pthreadF, NULL);
    cout <<"pt_1="<< pt_1<<endl;
    Aio a;
    a.test();
    a.tcp();
    return 0;
}