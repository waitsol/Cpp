#ifndef __ROLE_H
#define __ROLE_H
//前置声明  只能返回其指针  好处就是Role.cc定义发生改变 引用.h文件不需要重新编译， 坏处就是无法访问成员
struct Role;
Role *GetRole();
//未定义因为需要知道Role的类型申请空间 只能创建指针 让它链接的时候去找符号链接
// Role NewRole() aggregate ‘Role r’ has incomplete type and cannot be defined
// {
//     Role r;
//     return r;
// }
#endif