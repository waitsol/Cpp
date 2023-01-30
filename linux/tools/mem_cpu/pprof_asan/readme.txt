
g++ -o aa main.cpp -ltcmalloc -g -O0

env PPROF_PATH=/usr/local/bin/pprof LD_PRELOAD="/usr/local/lib/libtcmalloc.so" HEAPCHECK=normal  ./aa
执行玩上一句 会给你一些信息里面有这句 执行就行 --gv可以改换成--text pdf 等等其他输出格式， --lines可以改成函数格式，知道在那个函数出现了内存泄漏
pprof ./aa "/tmp/aa.287979._main_-end.heap" --inuse_objects --lines --heapcheck  --edgefraction=1e-10 --nodefraction=1e-10 --gv

