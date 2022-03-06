#ifndef ALLOC_H
#define ALLOC_H
#include "common.h"

#if 0
#include <new>
#define __THROW_BAD_ALLLOC throw bad_alloc
#elif !defined(__THROW_BAD_ALLLOC)
#include <iostream>
#define __THROW_BAD_ALLLOC                     \
    std::cerr << "out of memory" << std::endl; \
    exit(-1);
#endif

//一级适配器
template <int inst>
class __malloc_alloc_template
{
public:
    using malloc_alloc_oom_handler = void (*)();

public:
    ~__malloc_alloc_template() {}

public:
    static void *oom_malloc(size_t);
    static void *oom_realloc(void *, size_t);
    static malloc_alloc_oom_handler __malloc_alloc_oom_handler;
    // static void (*__malloc_alloc_oom_handler)();

public:
    static void *allocate(size_t n)
    {
        void *result = malloc(n);
        if (0 == result)
            result = oom_malloc(n);
        return result;
    }
    static void deallocate(void *p, size_t n)
    {
        free(p);
    }
    static void *reallocate(void *p, size_t n)
    {
        void *result = realloc(p, n);
        if (0 == result)
            result = oom_realloc(p, n);
        return result;
    }
    static malloc_alloc_oom_handler set_malloc_alloc_oom_handler(malloc_alloc_oom_handler hander)
    {
        malloc_alloc_oom_handler old = __malloc_alloc_oom_handler;
        __malloc_alloc_oom_handler = hander;
        return old;
    }
};

//二级适配器
#define ALIGN (8)
#define MAX_BYTES (128)
#define NFREELISTS (MAX_BYTES / ALIGN)

template <bool threads, int inst>
class __default_alloc_template
{
public:
private:
    static size_t RUOND_UP(size_t bytes)
    {
        //很巧妙 如果有余数 +align-1 必上升 然后清除掉add后的余数即口
        return (bytes + ALIGN - 1) & ~(ALIGN - 1); //将byttes上升至ALIGN的整数倍
    }
    union obj
    {
        union obj *free_list_link; //不为空的时候 client保证了足够的大小，这个指针指向了下一块内存
        char client_data[0];       //动态扩容写法 没有使用到 只是为了预留内存空间
    };

    static size_t FREELIST_INDEX(size_t bytes)
    {
        // 1-ALIGN一组
        return (bytes - 1) / ALIGN; //
        // stl源码写法 挺有意思  想让ALIGN和 1-ALIGn-1 一组
        //所以+Align-1导致 1-Align都上升 维度Align没有  最后-1就行
        // return （bytes-1+Align)/align -1;
    }

    static void *refile(size_t n);
    // nobj 传入传出参数 代表需要的数量和实际返还的数量
    static char *chunk_alloc(size_t n, int &nobj);

    // state
    static char *start_free; //内存迟位置标识
    static char *end_free;
    static size_t heap_size;
    //静态变量 共享 所以没考虑析构 程序结束就释放了  但是我测试发现容器离开作用域就释放了内存
    //现在c++版本中alloc使用的是普通的一级适配器，所以程序会释放内存，还有二级适配器普通版本存在内存安全问题
    //毕竟共享或许这也是模板中出现threads的问题
    static obj *volatile free_list[NFREELISTS];

public:
    static void *allocate(size_t n);
    static void *dealloacate(void *p, size_t n);
    // ...没讲好像也没用到 就没实现
    static void *reallocate(void *p, size_t old_sz, size_t new_sz);
};

#ifdef _USE_MALLOC
typedef __malloc_alloc_template<0> malloc_alloc;
typedef malloc_alloc alloc;
#else
typedef __default_alloc_template<false, 0> alloc;
#endif

template <class T, class Alloc>
class simple_alloc
{
public:
    static T *allocate(size_t n)
    {
        return 0 == 0 ? 0 : (T *)Alloc::allocatoe(n * sizeof(T));
    }
    static T *allocate()
    {
        return (T *)Alloc::allocatoe(sizeof(T));
    }
    static void deallocate(T *p, size_t n)
    {
        if (0 != n)
            Alloc::deallocate(p, n * sizeof(T));
    }

    static void deallocate(T *p)
    {

        Alloc::deallocate(p, sizeof(T));
    }
};

#endif