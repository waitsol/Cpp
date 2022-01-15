#include <stdio.h>
#include <iostream>
#include <string>
#include <cstring>
#include <time.h>
#include <unistd.h>
using namespace std;
#pragma pack(1)
struct str
{
	int len;
	char *s;
};

struct node
{
	int  xxx;   //4字节
	char yyy;   //1字节
	char data[0];   //零字节数组
};
const int size=1024*1024*100;
int main()
{
	char c = 'x';
    int t=10;
    while(t--)
    {
        node* p = (node*)malloc(sizeof(node)+size);//申请三个字节紧跟着node结构体

        cout << sizeof(node) << endl;
        p->xxx = 1;
        p->yyy = 2;
        p->data[0] = 'a';
        p->data[1] = 'b';
        p->data[2] = 'b';
        p->data[3] = 'b';
        p->data[4] = 0;
        memset((char*)p, 0, sizeof(node)+size);//在结构体里填零，方便对比
        cout << p->data << endl;
        free(p);
        sleep(1);
    }
	
	return 0;
}