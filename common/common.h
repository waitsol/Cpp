#ifndef __COMMON_H
#define __COMMON_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string.h>
#include <iostream>

#include <unordered_set>
#include <thread>
#include <functional>
#include <fcntl.h>
#ifndef WIN32
#include <sys/epoll.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif // !WIN32


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