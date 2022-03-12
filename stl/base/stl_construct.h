
#ifndef __SGI_STL_INTERNAL_CONSTRUCT_H
#define __SGI_STL_INTERNAL_CONSTRUCT_H

#include <new> // 需要placement new的原型
#include "traits.h"
template <class T>
inline void destroy(T *pointer)
{
    pointer->~T();
}

template <class T1, class T2>
inline void construct(T1 *p, const T2 &value)
{
    new (p) T1(value);
}

template <class ForwardIterator>
inline void
__destroy_aux(ForwardIterator first, ForwardIterator last, __false_type)
{
    for (; first < last; ++first)
        destroy(&*first);
}

template <class ForwardIterator>
inline void __destroy_aux(ForwardIterator, ForwardIterator, __true_type) {}

template <class ForwardIterator, class T>
inline void __destroy(ForwardIterator first, ForwardIterator last, T *)
{
    typedef typename __type_traits<T>::has_trivial_destructor trivial_destructor;
    __destroy_aux(first, last, trivial_destructor());
}

template <class ForwardIterator>
inline void destroy(ForwardIterator first, ForwardIterator last)
{
    __destroy(first, last, value_type(first));
}

// 特化版本
inline void destroy(char *, char *) {}
inline void destroy(wchar_t *, wchar_t *) {}

template <class InputIterator, class ForwardIterator>
inline ForwardIterator __uninitialized_copy_aux(InputIterator first, InputIterator last,
                                                ForwardIterator result,
                                                __true_type)
{
    return copy(first, last, result);
}

template <class InputIterator, class ForwardIterator>
ForwardIterator __uninitialized_copy_aux(InputIterator first, InputIterator last,
                                         ForwardIterator result,
                                         __false_type)
{
    ForwardIterator cur = result;

    for (; first != last; ++first, ++cur)
        construct(&*cur, *first);
    return cur;

    destroy(result, cur);
}

template <class InputIterator, class ForwardIterator, class T>
inline ForwardIterator __uninitialized_copy(InputIterator first, InputIterator last,
                                            ForwardIterator result, T *)
{
    typedef typename __type_traits<T>::is_POD_type is_POD;
    return __uninitialized_copy_aux(first, last, result, is_POD());
}

template <class InputIterator, class ForwardIterator>
inline ForwardIterator uninitialized_copy(InputIterator first, InputIterator last,
                                          ForwardIterator result)
{
    return __uninitialized_copy(first, last, result, value_type(result));
}

#endif 

