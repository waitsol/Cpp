#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <gperftools/profiler.h>
#include <random>
using namespace std;
const int mod = 1e9 + 7;
default_random_engine e(time(0));
uniform_int_distribution<int> u(0, 0x3f3f3f3f);
template <typename T>
int Partition(T *a, int left, int right)
{
    int i = left + 1, j = right;
    // default_random_engine e;
    // int k = e() % (right - left-1) + left;
    int mid = (left + right) / 2;
    int pivot = a[mid];
    swap(a[mid], a[left]);
    while (1)
    {
        while (i <= j && a[i] <= pivot)
            i++;
        while (i <= j && a[j] >= pivot)
            j--;
        if (i >= j)
            break;
        swap(a[i], a[j]);
    }
    swap(a[j], a[left]);
    return j;
}
template <typename T>
void quickSort(T *a, int left, int right)
{
    if (left < right)
    {
        int mid = Partition(a, left, right);
        quickSort(a, left, mid - 1);
        quickSort(a, mid + 1, right);
    }
}
template <typename T>
void Merge(T *a, int left, int center, int right)
{
    T *tmp = new T[right - left + 1];
    int i = left, j = center + 1;
    int idx = 0;
    for (int k = left; k <= right; k++)
    {
        if (i > center)
            tmp[idx++] = a[j++];
        else if (j > right)
            tmp[idx++] = a[i++];
        else if (a[i] > a[j])
            tmp[idx++] = a[j++];
        else
            tmp[idx++] = a[i++];
    }
    idx = 0;
    for (int k = left; k <= right; k++)
        a[k] = tmp[idx++];
    delete[] tmp;
}
template <typename T>
void MergeSort(T *a, int left, int right)
{
    if (left < right)
    {
        int mid = (left + right) / 2;
        MergeSort(a, left, mid);
        MergeSort(a, mid + 1, right);
        Merge(a, left, mid, right);
    }
}
void f1(int *a, int sz)
{

    quickSort(a, 0, sz - 1);
}
void f2(int *a, int sz)
{

    MergeSort(a, 0, sz - 1);
}

int main()
{
    int n = 10;
    int idx = 1;
    ProfilerStart((to_string(idx) + ".prof").data());
    int sz = 10000 * 10;
    int *a = new int[sz];
    int *b = new int[sz];
    for (int i = 0; i < sz; i++)
    {
        a[i] = u(e);
        b[i] = a[i];
    }
    while (n--)
    {

        f1(a, sz);
        f2(b, sz);
    }
    ProfilerStop();

    return 0;
}