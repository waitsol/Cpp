#ifndef __ZLK_BUFFER_H
#define __ZLK_BUFFER_H
#include <cstring>
#include <fstream>
#include <iostream>
using std::ofstream;
class zlk_buffer
{
public:
    zlk_buffer(int size = 4096) : m_size(size), m_data(new char[m_size]), m_curSize(0) {}
    const char *data() { return m_data; }
    int length() { return m_curSize; }
    int leave() { return m_size-m_curSize; }
    bool  append(const char *data, int n)
    {
        if(n+m_curSize>m_size)
            return false;
      //  std::cerr<<data;
        memcpy(m_data+m_curSize,data,n);
        m_curSize+=n;
        ++m_line;
        return true;
    }
    int write2File(ofstream &file)
    {
        file.write(m_data,m_curSize);
        return m_curSize;
    }
    void reset()
    {
        m_line=0;
        m_curSize=0;
        m_data[0]=0;
    }
    int getline()
    {
        return m_line;
    }
    ~zlk_buffer()
    {
        if (m_data)
        {
            delete[] m_data;
        }
    }

private:
    zlk_buffer(const zlk_buffer &) = delete;
    zlk_buffer operator=(const zlk_buffer &) = delete;

private:
    int m_size;
    char *m_data;
    int m_line=0;
    int m_curSize;
};

#endif