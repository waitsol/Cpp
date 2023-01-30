cd ~
wget https://github.com/libunwind/libunwind/releases/download/v1.6.2/libunwind-1.6.2.tar.gz
tar -zxvf libunwind-1.6.2.tar.gz
cd libunwind-1.6.2
./configure
make -j8
sudo make install
cd ~
rm -rf libunwind-1.6.2.tar.gz libunwind-1.6.2

cd ~
wget https://github.com/gperftools/gperftools/releases/download/gperftools-2.10/gperftools-2.10.tar.gz
tar -zxvf gperftools-2.10.tar.gz
cd gperftools-2.10
./configure
make -j8
sudo make install
cd ~
rm -rf gperftools-2.10.tar.gz gperftools-2.10

ai  graphviz
ai ghostscript

--------------------------------------------------------------------------------------------------------------------------------
因为pprof是一段程序性能测试 如果想测试一个时间段 可以使用进程间通信 打开  测试数据，信号 网络io 等等
编译 
g++ main.cpp  -lprofiler -g

//转换命令 需要先使用下面命令生成文件
pprof --pdf ./a.out 1.prof > 1.pdf
pprof --text ./a.out 1.prof > 1.text
text数据中
每行按照数据顺序：

分析样本数量（不包含其他函数调用）-- 断点在这边
分析样本百分比（不包含其他函数调用）
目前为止的分析样本百分比（不包含其他函数调用） --没搞懂这个
分析样本数量（包含其他函数调用）--断点在他调用的函数中
分析样本百分比（包含其他函数调用）
函数名



//下面这个方法我一直没成功 我看网上的人也没成功 所以采用第二种 在程序加在调试的断点
CPUPROFILE=./out ./a.out   (out是pprof产生的结果，a.out是你的可执行程序) 

#include <gperftools/profiler.h>
....
int main(int argc, const char* argv[])
{
    ProfilerStart("test_capture.prof");
    .....需要检测的函数
    ProfilerStop();
}