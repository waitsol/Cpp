#include <iostream>
using namespace std;
struct list_head
{
    struct list_head *next, *prev;
};
struct foo
{
    int fooness;
    struct list_head list;
};
// #define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

#define container_of(ptr, type, member) ({                      \
    const  typeof( ((type *)0)->member ) *__mptr = (ptr);    \
    (type *)( (char *)__mptr - offsetof(type,member) ); })

#define list_entry(ptr, type, member) \
    container_of(ptr, type, member)
#define list_for_each_entry(pos, head, member)                 \
    for (pos = list_entry((head)->next, typeof(*pos), member); \
         &pos->member != (head);                               \
         pos = list_entry(pos->member.next, typeof(*pos), member))
int main()
{
    list_head *head = new list_head;
    foo *iter;
    foo *fir = new foo;
    fir->fooness = 1;
    foo *sec = new foo;
    sec->fooness = 2;
    fir->list.next = &sec->list;
    sec->list.prev = &fir->list;
    sec->list.next = (list_head *)head;
    head->next = &fir->list;

    list_for_each_entry(iter, head, list)
            cout
        << iter->fooness << endl;

    for (int i = ({int x=4,y=2;x-4;y; }); i < 3; i++)
    {
        cout << i << endl;
    }

    int i = ({int x=4,y=2;x-4;y; });
    cout << i << endl;

    return 0;
}