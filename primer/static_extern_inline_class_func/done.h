#ifndef DONE_H
#define DONE_H 1
#include "inc.h"
#include "stdio.h"
class done
{
private:
    /* data */
public:
    done(/* args */);
    ~done();

    void done1()
    {
        printf("done1 inc_static_a = %d addr=%p\n",++inc_static_a,&inc_static_a);
    }
    void done2();

};




#endif