#include "common.h"
#include "alloc.h"

template <int inst>
typename __malloc_alloc_template<inst>::malloc_alloc_oom_handler //返回值类型 需要typename表示是类型
    __malloc_alloc_template<inst>::__malloc_alloc_oom_handler = 0;

template <int inst>
void *__malloc_alloc_template<inst>::oom_malloc(size_t n)
{
    malloc_alloc_oom_handler handler = __malloc_alloc_oom_handler;
    void *result;
    for (;;)
    {
        if (0 == handler)
        {
            __THROW_BAD_ALLLOC;
        }
        handler();
        result = malloc(n);
        if (result)
            return result;
    }
}
template <int inst>
void *__malloc_alloc_template<inst>::oom_realloc(void *src, size_t n)
{
    malloc_alloc_oom_handler handler = __malloc_alloc_oom_handler;
    void *result;
    for (;;)
    {
        if (0 == handler)
        {
            __THROW_BAD_ALLLOC;
        }
        handler();
        result = realloc(src, n);
        if (result)
            return result;
    }
}
typedef __malloc_alloc_template<0> malloc_alloc;
template <bool threads, int inst>
char *__default_alloc_template<threads, inst>::start_free = 0;

template <bool threads, int inst>
char *__default_alloc_template<threads, inst>::end_free = 0;

template <bool threads, int inst>
size_t __default_alloc_template<threads, inst>::heap_size = 0;

template <bool threads, int inst>
typename __default_alloc_template<threads, inst>::obj *volatile __default_alloc_template<threads, inst>::free_list[NFREELISTS] = {0};

template <bool threads, int inst>
void *__default_alloc_template<threads, inst>::allocate(size_t n)
{
    obj *result;
    obj *volatile *my_free_list;
    if (n > MAX_BYTES)
    {
        return malloc_alloc::allocate(n);
    }
    my_free_list = free_list + FREELIST_INDEX(n);
    result = *my_free_list; //取n对应的内存链表 =free_list[FREELIST_INDEX(n)]
    if (my_free_list == nullptr)
    {
        void *r = refile(RUOND_UP(n));
        return r;
    }
    *my_free_list = result->free_list_link; //内存链表下移
    return result;
}
template <bool threads, int inst>
void *__default_alloc_template<threads, inst>::dealloacate(void *p, size_t n)
{
    obj *q = (obj *)p;
    obj *volatile *my_free_list;
    if (n > MAX_BYTES)
    {
        return malloc_alloc::deallocate(p,n);
    }
    my_free_list = free_list + FREELIST_INDEX(n);

    q->free_list_link = *my_free_list;
    *my_free_list = q;
}
template <bool threads, int inst>
void *__default_alloc_template<threads, inst>::refile(size_t n)
{
    int nobj = 20;
    char *chunk = chunk_alloc(n, nobj);

    obj *volatile *my_free_list;
    obj *result;
    obj *cur_obj, next_obj;

    if (nobj == 1)
    {
        //内存池只剩下一个了，链表不用更新
        return chunk;
    }
    my_free_list = free_list + FREELIST_INDEX(n);
    result = (obj *)chunk;
    //前n个字节返还  指针后移，剩下的维护到链表
    *my_free_list = next_obj = (obj *)(chunk + n);
    for (int i = 1; i < nobj; i++)
    {
        cur_obj = next_obj;

        if (i + 1 == nobj)
        {
            cur_obj->free_list_link = 0;
        }
        else
        {
            //维护链表 指针继续后移
            next_obj = (obj *)(chunk + n);
            cur_obj->free_list_link = next_obj;
        }
    }
    return result;
}
template <bool threads, int inst>
char *__default_alloc_template<threads, inst>::chunk_alloc(size_t n, int &nobj)
{
    size_t total_size = n * nobj;
    size_t left_size = end_free - start_free;
    char *result;
    if (left_size >= total_size)
    {
        result = start_free;
        start_free += total_size;
        return result;
    }
    else if (left_size >= n)
    {
        nobj = left_size / n;
        result = start_free;
        start_free += nobj * n;
        return result;
    }
    else
    {
        size_t byte_to_get = 2 * total_size + RUOND_UP(heap_size >> 4);
        if (left_size > 0)
        {
            //将剩余的内存维护到链表
            obj *volatile *my_free_list = my_free_list = free_list + FREELIST_INDEX(left_size);

            ((obj *)start_free)->free_list_link = *my_free_list;
            *my_free_list = (obj *)start_free;
        }
        start_free = (char *)malloc(byte_to_get);
        if (start_free == nullptr)
        {
            obj *volatile *my_free_list, *p;
            //看看n后面的节点有没有剩余
            for (int i = n; i < MAX_BYTES; i += ALIGN)
            {
                my_free_list = free_list + FREELIST_INDEX(i);
                p = *my_free_list;
                if (*my_free_list != nullptr)
                {
                    *my_free_list = p->free_list_link;
                    start_free = (char *)p;
                    end_free = start_free + i;
                    return chunk_alloc(n, nobj);
                }
            }
            end_free = 0;
            //看看oom机制
            start_free = (char *)::malloc_alloc::allocate(byte_to_get);
        }
        heap_size += byte_to_get;
        end_free = start_free + byte_to_get;
        //内存申请好了 继续递归 返还nobj
        return chunk_alloc(n, nobj);
    }
}
int main()
{
    return 0;
}