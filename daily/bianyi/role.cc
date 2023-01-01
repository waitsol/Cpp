#include "role.h"
struct Role
{
    /* data */
    int roleid;
    char* name;
};

Role *GetRole()
{
    return new Role();
}