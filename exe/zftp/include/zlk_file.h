#ifndef __ZLK_FILE
#define __ZLK_FILE
#include <vector>
#include <string>
using namespace std;
#ifdef WIN32
#include <io.h>
vector<string> getFiles(string path, bool dg = false)
{

    // 文件句柄
    long hFile = 0;
    // 文件信息
    struct _finddata_t fileinfo;
    string p;
    vector<string> files;
    if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
    {
        do
        {
            // 如果是目录,迭代之
            // 如果不是,加入列表
            if ((fileinfo.attrib & _A_SUBDIR))
            {
                if (dg)
                {
                    if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
                    {
                        auto res = getFiles(p.assign(path).append("\\").append(fileinfo.name), dg);

                        for (auto s : res)
                            files.push_back(s);
                    }
                }
                else
                {
                    files.push_back(fileinfo.name);
                }
            }
            else
            {
                files.push_back(fileinfo.name);
            }
        } while (_findnext(hFile, &fileinfo) == 0);
        _findclose(hFile);
    }
    return files;
}
#else
#include <string>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

vector<string> getFiles(string path)
{
    DIR *pDir;
    struct dirent *ptr;
    vector<string> filenames;
    if (!(pDir = opendir(path.c_str())))
    {
        return filenames;
    }
    while ((ptr = readdir(pDir)) != 0)
    {
        if (strcmp(ptr->d_name, ".") != 0 && strcmp(ptr->d_name, "..") != 0)
        {
            filenames.push_back(ptr->d_name);
        }
    }
    closedir(pDir);
    return filenames;
}
#endif

#endif