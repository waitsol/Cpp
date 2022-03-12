#include "common.h"
#include "alloc.h"
#include "stl_iterator.h"
#include "stl_construct.h"
namespace _STL
{
    template <class T, class Alloc = alloc>
    class vector_stl
    {
    public:
        typedef T value_type;
        typedef value_type *pointer;
        typedef value_type *iterator;
        typedef value_type &reference;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;

    public:
        vector_stl(/* args */);
        ~vector_stl();

        typedef simple_alloc<value_type, Alloc> date_allocator;

        iterator start;
        iterator finish;
        iterator end_of_storage;
        void insert_aux(iterator, const T &);
        void deallocate()
        {
            if (start)
                data_allocator::deallocator(start, end_of_storage - start);
        }
        void fill_initialize(size_type n, const T &value)
        {
            start = allocator_and_fill(n, value);
            finish = start + n;
            end_of_storage = finish;
        }
        iterator begin() { return start; }
        iterator end() { return finish; }
        size_type size(){return size_type(end() - begin())} size_type capacity() { return end_of_storage - begin() }
        bool empty() { return begin() == end(); }
        reference opertaor[](size_type n){return *(begin() + n)};
        vector_stl() : start(0), finish(0), end_of_storage(0) {}
        vector_stl(size_type n, const T &value) { fill_initialize(n, value) }
        explicit vector_stl(size_type n){fill_initialize(n, 0)}

        ~vector_stl()
        {
            destroy(start,finish);
            deallocate();
        }
        reference front(){return *begin()};
        reference back() { return *(end() - 1) }
        
        void push_back(const T& x)
        {
            if(finish!=end_of_storage)
            {
                construct(finish,x);
                ++finish;
            }else{
                insert_aux(end(),x);
            }
        }
        void insert(iterator position,const T& x)
        {
            insert_aux(position,x);
        }
        void pop_back()
        {
            --finish;
            destroy(finish);
        }
        iterator erase(iterator position)
        {
            if(position+1!=end())
            {
                copy(position+1,finish,position);
                
            }
            --finish;
            destroy(finish);
            return position;
        }
        
    protected:
        iterator allocator_and_fill(size_type n, const T &x)
        {
            iterator result=date_allocator::allocate(n);
            //
        }
    };

}