#ifndef __LIST_H_
#define __LIST_H_
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
	typedef simple_alloc<list_node, Alloc> list_node_allocator;
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
	void swap(list_stl<T, alloc> &x)
	{
		std::swap(x.node, node);
	}
public:
	void push_back(const T&x) { insert(end(), x); }
	iterator insert(iterator position, const T& x)
	{
		link_type t = create_node(x);
		t->next = position.node;
		t->prev = position.noee->prev;
		position.noee->prev->next = t;
		position.noee->prev = t;
		return t;
	}
	iterator earse(iterator position)
	{
		link_type next_node = position.node->next;
		link_type prev_node = position.node->prev;

		prev_node->next = next_node;
		next_node->prev = prev_node;
		destory_node(position.node);


		return t;
	}
	bool empty()
	{
		return size() == 0;
	}
protected:
	link_type get_node() { return list_node_allocator::allocate(); }
	void put_node(link_type p) { list_node_allocator::deallocate(p); }
	link_type create_node(const T& v)
	{
		link_type p = get_node();
		construct(&p->data, x);
		return x;
	}
	void destory_node(link_type p)
	{
		destory(&p->data);
		put_node(p);
	}

	void transfer(iterator position, iterator first, iterator last);
	void splice(iterator positionm, list_stl &x);
	void splice(iterator positionm, list_stl &x, iterator i);
	void splice(iterator positionm, list_stl &x,iterator first,iterator last);
	void merge(list_stl<T, Alloc> &x);
	void sort();
private:
    link_type node;
};

#endif

