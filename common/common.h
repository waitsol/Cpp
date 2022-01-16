#ifndef __COMMON_H
#define __COMMON_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <vector>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <sys/epoll.h>
#include <unordered_set>
#include <thread>
#include <functional>
#include <fcntl.h>
#define ERR(condition)                        \
    do                                        \
    {                                         \
        if (!(condition))                     \
        {                                     \
            printf("error %s\n", #condition); \
            goto EXIT;                        \
        }                                     \
    } while (0);
#endif