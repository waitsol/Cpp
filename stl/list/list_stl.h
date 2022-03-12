#ifndef __LIST_H
#define __LIST_H
#include "alloc.h"
//list节点
template<class T>
struct __list_node
{
    typedef __list_node *node_point;
    node_point prev;
    node_point next;
    T data;
};

//iter
template<class T>
struct __list_iterator
{

    typedef __list_iterator<T> self;
    typedef __list_iterator<T> iterator;

    typedef T value_type;
    typedef T* pointer;
    typedef T& reference;
    typedef __list_node<T>* link_type;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

    link_type node;
    
    __list_iterator(link_type x):node(x){}
    __list_iterator(){}
    __list_iterator(const __list_iterator& x):node(x.node){}

    bool operator!=(const self &x) const
    {
        return x.node != node;
    }
    bool operator==(const self &x) const
    {
        return x.node==node;
    }

    reference operator*()
    {
        return node->data;
    }
    pointer operator->()
    {
        return &operator*();
    } 
    self& operator++(){
        node=node->next;
        return *this;
    }
    self &operator++(int)
    {
        self tmp=*this;
        ++this;
        return self;
    }
    self &operator--()
    {
        node = node->prev;
        return *this;
    }
    self &operator--(int)
    {
        self tmp = *this;
        ++this;
        return self;
    }
};

template <class T, class Alloc = alloc>
    class list_stl : public  __list_iterator<T>{
protected:
    typedef __list_node<T> list_node;
public:
    typedef list_node* link_type;

    iterator begin()
    {
        return node->next;
    }
    iterator end() //md这里返回的是个iterator ....
    {
        return node;
    }
    bool empty()
    {
        return node==node->next;
    }
    size_type size()const{
        size_tyep result=0;
        return result;
    }
    reference front(){return *begin();}

    reference back()
    {
        return *(--end());
    }

private:
    link_type node;
}

#endif