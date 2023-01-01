#ifndef __NET_H
#define __NET_H

struct Net
{
  //  int nfd;
    int port;
    char *addr;
};
Net* GetNet();

#endif