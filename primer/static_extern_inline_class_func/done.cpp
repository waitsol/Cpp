#include "done.h"
#include "stdio.h"
int g_a=2;
done::done(/* args */)
{
}

done::~done()
{

}

void done::done2()
{
    printf("done2 inc_static_b = %d addr=%p\n", ++inc_static_b, &inc_static_b);
}