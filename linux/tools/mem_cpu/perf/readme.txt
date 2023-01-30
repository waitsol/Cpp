
https://www.cnblogs.com/arnoldlu/p/6241297.html

perf record -F 99  -g -p `ps aux | grep "a.out" | grep -v grep | cut -c 9-16` --sleep 2

 perf script > out.perf

//火焰图git
 https://github.com/brendangregg/FlameGraph.git
 生成火焰图方法
perf record -F 99 -p 16637 -g -- sleep 30  #记录数据
perf script > out.perf
./FlameGraph-master/stackcollapse-perf.pl out.perf > out.folded
./FlameGraph-master/flamegraph.pl out.folded > perf.svg
