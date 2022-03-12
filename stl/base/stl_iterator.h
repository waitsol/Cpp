#ifndef __STL_ITERATOR_H
#define __STL_ITERATOR_H
//五种迭代器，作为标记型别（tag types）,不需要任何成员
#include <cstddef>
struct input_iterator_tag{};
struct output_iterator_tag{};
struct forward_iterator_tag : public input_iterator_tag{};
struct bidirectional_iterator_tag : public forward_iterator_tag{};
struct random_access_iterator_tag : public bidirectional_iterator_tag{};

template <class T, class Distance>
struct input_iterator
{
    typedef input_iterator_tag iterator_category;
    typedef T value_type;
    typedef Distance difference_type;
    typedef T *pointer;
    typedef T &reference;
};

struct output_iterator
{
    typedef output_iterator_tag iterator_category;
    typedef void value_type;
    typedef void difference_type;
    typedef void pointer;
    typedef void reference;
};

template <class T, class Distance>
struct forward_iterator
{
    typedef forward_iterator_tag iterator_category;
    typedef T value_type;
    typedef Distance difference_type;
    typedef T *pointer;
    typedef T &reference;
};

template <class T, class Distance>
struct bidirectional_iterator
{
    typedef bidirectional_iterator_tag iterator_category;
    typedef T value_type;
    typedef Distance difference_type;
    typedef T *pointer;
    typedef T &reference;
};

template <class T, class Distance>
struct random_access_iterator
{
    typedef random_access_iterator_tag iterator_category;
    typedef T value_type;
    typedef Distance difference_type;
    typedef T *pointer;
    typedef T &reference;
};

template <class Category, class T, class Distance = ptrdiff_t,
          class Pointer = T *, class Reference = T &>
struct iterator
{
    typedef Category iterator_category;
    typedef T value_type;
    typedef Distance difference_type;
    typedef Pointer pointer;
    typedef Reference reference;
};

template <class Iterator>
struct iterator_traits
{
    // 迭代器类型, STL提供五种迭代器
    typedef  Iterator::iterator_category iterator_category;

    // 迭代器所指对象的型别
    // 如果想与STL算法兼容, 那么在类内需要提供value_type定义
    typedef  Iterator::value_type value_type;

    // 这个是用于处理两个迭代器间距离的类型
    typedef  Iterator::difference_type difference_type;

    // 直接指向对象的原生指针类型
    typedef  Iterator::pointer pointer;

    // 这个是对象的引用类型
    typedef  Iterator::reference reference;
};

// 针对指针提供特化版本
template <class T>
struct iterator_traits<T *>
{
    typedef random_access_iterator_tag iterator_category;
    typedef T value_type;
    typedef ptrdiff_t difference_type;
    typedef T *pointer;
    typedef T &reference;
};

// 针对指向常对象的指针提供特化
template <class T>
struct iterator_traits<const T *>
{
    typedef random_access_iterator_tag iterator_category;
    typedef T value_type;
    typedef ptrdiff_t difference_type;
    typedef const T *pointer;
    typedef const T &reference;
};

template <class Iterator>
inline typename iterator_traits<Iterator>::iterator_category
iterator_category(const Iterator &)
{
    typedef typename iterator_traits<Iterator>::iterator_category category;
    return category();
}

template <class Iterator>
inline typename iterator_traits<Iterator>::difference_type *
distance_type(const Iterator &)
{
    return static_cast<typename iterator_traits<Iterator>::difference_type *>(0);
}

template <class Iterator>
inline typename iterator_traits<Iterator>::value_type *
value_type(const Iterator &)
{
    return static_cast<typename iterator_traits<Iterator>::value_type *>(0);
}


template <class T, class Distance>
inline input_iterator_tag
iterator_category(const input_iterator<T, Distance> &)
{
    return input_iterator_tag();
}

inline output_iterator_tag iterator_category(const output_iterator &)
{
    return output_iterator_tag();
}

template <class T, class Distance>
inline forward_iterator_tag
iterator_category(const forward_iterator<T, Distance> &)
{
    return forward_iterator_tag();
}

template <class T, class Distance>
inline bidirectional_iterator_tag
iterator_category(const bidirectional_iterator<T, Distance> &)
{
    return bidirectional_iterator_tag();
}

template <class T, class Distance>
inline random_access_iterator_tag
iterator_category(const random_access_iterator<T, Distance> &)
{
    return random_access_iterator_tag();
}

template <class T>
inline random_access_iterator_tag iterator_category(const T *)
{
    return random_access_iterator_tag();
}

template <class T, class Distance>
inline T *value_type(const input_iterator<T, Distance> &)
{
    return (T *)(0);
}

template <class T, class Distance>
inline T *value_type(const forward_iterator<T, Distance> &)
{
    return (T *)(0);
}

template <class T, class Distance>
inline T *value_type(const bidirectional_iterator<T, Distance> &)
{
    return (T *)(0);
}

template <class T, class Distance>
inline T *value_type(const random_access_iterator<T, Distance> &)
{
    return (T *)(0);
}

template <class T>
inline T *value_type(const T *) { return (T *)(0); }

template <class T, class Distance>
inline Distance *distance_type(const input_iterator<T, Distance> &)
{
    return (Distance *)(0);
}

template <class T, class Distance>
inline Distance *distance_type(const forward_iterator<T, Distance> &)
{
    return (Distance *)(0);
}

template <class T, class Distance>
inline Distance *
distance_type(const bidirectional_iterator<T, Distance> &)
{
    return (Distance *)(0);
}

template <class T, class Distance>
inline Distance *
distance_type(const random_access_iterator<T, Distance> &)
{
    return (Distance *)(0);
}

template <class T>
inline ptrdiff_t *distance_type(const T *) { return (ptrdiff_t *)(0); }
template <class BidirectionalIterator1, class BidirectionalIterator2>
BidirectionalIterator2 copy_backward(BidirectionalIterator1 first, BidirectionalIterator1 last,
                                     BidirectionalIterator2 result)
{
    while (last != first)
    {
        *(--result) = *(--last);
    }
    return *result;
}


#endif