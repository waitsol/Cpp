#ifndef __TCPHEAD_H
#define __TCPHEAD_H
#include "common.h"
#ifdef __cplusplus
extern "C"
{
#endif
    extern int GetSockFd(std::string addr = "127.0.0.1", int port = 777);
    extern int GetSockBlockFd(std::string addr = "127.0.0.1", int port = 777);
#ifndef __cplusplus
}
#endif

#endif