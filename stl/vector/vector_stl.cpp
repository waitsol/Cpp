#include "vector_stl.h"
using namespace _STL;
template <class T, class Alloc>
void vector_stl<T,Alloc>::insert_aux(iterator position, const T &x)
{
    if(finish!=end_of_storage)//水位空间
    {
        construct(finish,*(finish-1)); 
        ++finish;
        copy_backward(position, finish - 2, finish - 1); // position后面插入 
        *position=x;
    }else{
        
        const size_type old_size=size();
        const size_type len=old_size==0?1:2*old_size;
        iterator new_start=date_allocator::allocator(len);
        iterator new_finish=new_start;

        try{
            //前半部copy
            new_finish=uninitialized_copy(start,position,new_start);
            construct(new_finish,x);
            ++new_finish;
            new_finish = uninitialized_copy(position, finish, new_finish);
        }catch(...){
            destroy(new_start,new_finish);
            date_allocator::dealloc(new_start,len);
            throw;
        }
        destroy(begin(),end());
        deallocate();
        start=new_start;
        finish=new_finish;
        end_of_storage=new_start+len;
    }
}
