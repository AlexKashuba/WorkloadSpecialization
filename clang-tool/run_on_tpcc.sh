PATH=".."
set -x
cmake-build-debug/bin/clang-tool  $PATH/DBx1000/benchmarks/tpcc_txn.cpp \
    -- -lpthread -D NOGRAPHITE=1 -std=c++11 -I/usr/local/include/c++/10.2.0 -I/usr/local/include/c++/10.2.0/x86_64-apple-darwin19 \
    -I/usr/local/include/boost -I$PATH/DBx1000 -I$PATH/DBx1000/benchmarks -I$PATH/DBx1000/concurrency_control -I$PATH/DBx1000/storage -I$PATH/DBx1000/system -I$PATH/DBx1000/include \
    /
