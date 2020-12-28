EXTRA_FLAGS="-I/usr/local/include/c++/10.2.0 -I/usr/local/include/c++/10.2.0/x86_64-apple-darwin19 -I/usr/local/include/boost -I/usr/local/Cellar/llvm/11.0.0/lib/clang/11.0.0/include"

cat << EOF
[
{
  "directory": "${PROJECT_PATH}/DBx1000/build",
  "command": "clang++  -DNOGRAPHITE=1 ${EXTRA_FLAGS} -I${PROJECT_PATH}/DBx1000 -I${PROJECT_PATH}/DBx1000/benchmarks -I${PROJECT_PATH}/DBx1000/concurrency_control -I${PROJECT_PATH}/DBx1000/storage -I${PROJECT_PATH}/DBx1000/system  -pthread -std=c++11 -Wno-deprecated-declarations   -o ${BUILD_DIR}/benchmarks/test_txn.cpp.o -c ${PROJECT_PATH}/DBx1000/benchmarks/test_txn.cpp",
  "file": "${PROJECT_PATH}/DBx1000/benchmarks/test_txn.cpp"
},
{
  "directory": "${PROJECT_PATH}/DBx1000/build",
  "command": "clang++  -DNOGRAPHITE=1 ${EXTRA_FLAGS} -I${PROJECT_PATH}/DBx1000 -I${PROJECT_PATH}/DBx1000/benchmarks -I${PROJECT_PATH}/DBx1000/concurrency_control -I${PROJECT_PATH}/DBx1000/storage -I${PROJECT_PATH}/DBx1000/system  -pthread -std=c++11 -Wno-deprecated-declarations   -o ${BUILD_DIR}/benchmarks/test_wl.cpp.o -c ${PROJECT_PATH}/DBx1000/benchmarks/test_wl.cpp",
  "file": "${PROJECT_PATH}/DBx1000/benchmarks/test_wl.cpp"
},
{
  "directory": "${PROJECT_PATH}/DBx1000/build",
  "command": "clang++  -DNOGRAPHITE=1 ${EXTRA_FLAGS} -I${PROJECT_PATH}/DBx1000 -I${PROJECT_PATH}/DBx1000/benchmarks -I${PROJECT_PATH}/DBx1000/concurrency_control -I${PROJECT_PATH}/DBx1000/storage -I${PROJECT_PATH}/DBx1000/system  -pthread -std=c++11 -Wno-deprecated-declarations   -o ${BUILD_DIR}/benchmarks/tpcc_helper.cpp.o -c ${PROJECT_PATH}/DBx1000/benchmarks/tpcc_helper.cpp",
  "file": "${PROJECT_PATH}/DBx1000/benchmarks/tpcc_helper.cpp"
},
{
  "directory": "${PROJECT_PATH}/DBx1000/build",
  "command": "clang++  -DNOGRAPHITE=1 ${EXTRA_FLAGS} -I${PROJECT_PATH}/DBx1000 -I${PROJECT_PATH}/DBx1000/benchmarks -I${PROJECT_PATH}/DBx1000/concurrency_control -I${PROJECT_PATH}/DBx1000/storage -I${PROJECT_PATH}/DBx1000/system  -pthread -std=c++11 -Wno-deprecated-declarations   -o ${BUILD_DIR}/benchmarks/tpcc_query.cpp.o -c ${PROJECT_PATH}/DBx1000/benchmarks/tpcc_query.cpp",
  "file": "${PROJECT_PATH}/DBx1000/benchmarks/tpcc_query.cpp"
},
{
  "directory": "${PROJECT_PATH}/DBx1000/build",
  "command": "clang++  -DNOGRAPHITE=1 ${EXTRA_FLAGS} -I${PROJECT_PATH}/DBx1000 -I${PROJECT_PATH}/DBx1000/benchmarks -I${PROJECT_PATH}/DBx1000/concurrency_control -I${PROJECT_PATH}/DBx1000/storage -I${PROJECT_PATH}/DBx1000/system  -pthread -std=c++11 -Wno-deprecated-declarations   -o ${BUILD_DIR}/benchmarks/tpcc_txn.cpp.o -c ${PROJECT_PATH}/DBx1000/benchmarks/tpcc_txn.cpp",
  "file": "${PROJECT_PATH}/DBx1000/benchmarks/tpcc_txn.cpp"
},
{
  "directory": "${PROJECT_PATH}/DBx1000/build",
  "command": "clang++  -DNOGRAPHITE=1 ${EXTRA_FLAGS} -I${PROJECT_PATH}/DBx1000 -I${PROJECT_PATH}/DBx1000/benchmarks -I${PROJECT_PATH}/DBx1000/concurrency_control -I${PROJECT_PATH}/DBx1000/storage -I${PROJECT_PATH}/DBx1000/system  -pthread -std=c++11 -Wno-deprecated-declarations   -o ${BUILD_DIR}/benchmarks/tpcc_wl.cpp.o -c ${PROJECT_PATH}/DBx1000/benchmarks/tpcc_wl.cpp",
  "file": "${PROJECT_PATH}/DBx1000/benchmarks/tpcc_wl.cpp"
},
{
  "directory": "${PROJECT_PATH}/DBx1000/build",
  "command": "clang++  -DNOGRAPHITE=1 ${EXTRA_FLAGS} -I${PROJECT_PATH}/DBx1000 -I${PROJECT_PATH}/DBx1000/benchmarks -I${PROJECT_PATH}/DBx1000/concurrency_control -I${PROJECT_PATH}/DBx1000/storage -I${PROJECT_PATH}/DBx1000/system  -pthread -std=c++11 -Wno-deprecated-declarations   -o ${BUILD_DIR}/benchmarks/ycsb_query.cpp.o -c ${PROJECT_PATH}/DBx1000/benchmarks/ycsb_query.cpp",
  "file": "${PROJECT_PATH}/DBx1000/benchmarks/ycsb_query.cpp"
},
{
  "directory": "${PROJECT_PATH}/DBx1000/build",
  "command": "clang++  -DNOGRAPHITE=1 ${EXTRA_FLAGS} -I${PROJECT_PATH}/DBx1000 -I${PROJECT_PATH}/DBx1000/benchmarks -I${PROJECT_PATH}/DBx1000/concurrency_control -I${PROJECT_PATH}/DBx1000/storage -I${PROJECT_PATH}/DBx1000/system  -pthread -std=c++11 -Wno-deprecated-declarations   -o ${BUILD_DIR}/benchmarks/ycsb_txn.cpp.o -c ${PROJECT_PATH}/DBx1000/benchmarks/ycsb_txn.cpp",
  "file": "${PROJECT_PATH}/DBx1000/benchmarks/ycsb_txn.cpp"
},
{
  "directory": "${PROJECT_PATH}/DBx1000/build",
  "command": "clang++  -DNOGRAPHITE=1 ${EXTRA_FLAGS} -I${PROJECT_PATH}/DBx1000 -I${PROJECT_PATH}/DBx1000/benchmarks -I${PROJECT_PATH}/DBx1000/concurrency_control -I${PROJECT_PATH}/DBx1000/storage -I${PROJECT_PATH}/DBx1000/system  -pthread -std=c++11 -Wno-deprecated-declarations   -o ${BUILD_DIR}/benchmarks/ycsb_wl.cpp.o -c ${PROJECT_PATH}/DBx1000/benchmarks/ycsb_wl.cpp",
  "file": "${PROJECT_PATH}/DBx1000/benchmarks/ycsb_wl.cpp"
},
{
  "directory": "${PROJECT_PATH}/DBx1000/build",
  "command": "clang++  -DNOGRAPHITE=1 ${EXTRA_FLAGS} -I${PROJECT_PATH}/DBx1000 -I${PROJECT_PATH}/DBx1000/benchmarks -I${PROJECT_PATH}/DBx1000/concurrency_control -I${PROJECT_PATH}/DBx1000/storage -I${PROJECT_PATH}/DBx1000/system  -pthread -std=c++11 -Wno-deprecated-declarations   -o ${BUILD_DIR}/concurrency_control/dl_detect.cpp.o -c ${PROJECT_PATH}/DBx1000/concurrency_control/dl_detect.cpp",
  "file": "${PROJECT_PATH}/DBx1000/concurrency_control/dl_detect.cpp"
},
{
  "directory": "${PROJECT_PATH}/DBx1000/build",
  "command": "clang++  -DNOGRAPHITE=1 ${EXTRA_FLAGS} -I${PROJECT_PATH}/DBx1000 -I${PROJECT_PATH}/DBx1000/benchmarks -I${PROJECT_PATH}/DBx1000/concurrency_control -I${PROJECT_PATH}/DBx1000/storage -I${PROJECT_PATH}/DBx1000/system  -pthread -std=c++11 -Wno-deprecated-declarations   -o ${BUILD_DIR}/concurrency_control/occ.cpp.o -c ${PROJECT_PATH}/DBx1000/concurrency_control/occ.cpp",
  "file": "${PROJECT_PATH}/DBx1000/concurrency_control/occ.cpp"
},
{
  "directory": "${PROJECT_PATH}/DBx1000/build",
  "command": "clang++  -DNOGRAPHITE=1 ${EXTRA_FLAGS} -I${PROJECT_PATH}/DBx1000 -I${PROJECT_PATH}/DBx1000/benchmarks -I${PROJECT_PATH}/DBx1000/concurrency_control -I${PROJECT_PATH}/DBx1000/storage -I${PROJECT_PATH}/DBx1000/system  -pthread -std=c++11 -Wno-deprecated-declarations   -o ${BUILD_DIR}/concurrency_control/plock.cpp.o -c ${PROJECT_PATH}/DBx1000/concurrency_control/plock.cpp",
  "file": "${PROJECT_PATH}/DBx1000/concurrency_control/plock.cpp"
},
{
  "directory": "${PROJECT_PATH}/DBx1000/build",
  "command": "clang++  -DNOGRAPHITE=1 ${EXTRA_FLAGS} -I${PROJECT_PATH}/DBx1000 -I${PROJECT_PATH}/DBx1000/benchmarks -I${PROJECT_PATH}/DBx1000/concurrency_control -I${PROJECT_PATH}/DBx1000/storage -I${PROJECT_PATH}/DBx1000/system  -pthread -std=c++11 -Wno-deprecated-declarations   -o ${BUILD_DIR}/concurrency_control/row_lock.cpp.o -c ${PROJECT_PATH}/DBx1000/concurrency_control/row_lock.cpp",
  "file": "${PROJECT_PATH}/DBx1000/concurrency_control/row_lock.cpp"
},
{
  "directory": "${PROJECT_PATH}/DBx1000/build",
  "command": "clang++  -DNOGRAPHITE=1 ${EXTRA_FLAGS} -I${PROJECT_PATH}/DBx1000 -I${PROJECT_PATH}/DBx1000/benchmarks -I${PROJECT_PATH}/DBx1000/concurrency_control -I${PROJECT_PATH}/DBx1000/storage -I${PROJECT_PATH}/DBx1000/system  -pthread -std=c++11 -Wno-deprecated-declarations   -o ${BUILD_DIR}/concurrency_control/row_mvcc.cpp.o -c ${PROJECT_PATH}/DBx1000/concurrency_control/row_mvcc.cpp",
  "file": "${PROJECT_PATH}/DBx1000/concurrency_control/row_mvcc.cpp"
},
{
  "directory": "${PROJECT_PATH}/DBx1000/build",
  "command": "clang++  -DNOGRAPHITE=1 ${EXTRA_FLAGS} -I${PROJECT_PATH}/DBx1000 -I${PROJECT_PATH}/DBx1000/benchmarks -I${PROJECT_PATH}/DBx1000/concurrency_control -I${PROJECT_PATH}/DBx1000/storage -I${PROJECT_PATH}/DBx1000/system  -pthread -std=c++11 -Wno-deprecated-declarations   -o ${BUILD_DIR}/concurrency_control/row_occ.cpp.o -c ${PROJECT_PATH}/DBx1000/concurrency_control/row_occ.cpp",
  "file": "${PROJECT_PATH}/DBx1000/concurrency_control/row_occ.cpp"
},
{
  "directory": "${PROJECT_PATH}/DBx1000/build",
  "command": "clang++  -DNOGRAPHITE=1 ${EXTRA_FLAGS} -I${PROJECT_PATH}/DBx1000 -I${PROJECT_PATH}/DBx1000/benchmarks -I${PROJECT_PATH}/DBx1000/concurrency_control -I${PROJECT_PATH}/DBx1000/storage -I${PROJECT_PATH}/DBx1000/system  -pthread -std=c++11 -Wno-deprecated-declarations   -o ${BUILD_DIR}/concurrency_control/row_ts.cpp.o -c ${PROJECT_PATH}/DBx1000/concurrency_control/row_ts.cpp",
  "file": "${PROJECT_PATH}/DBx1000/concurrency_control/row_ts.cpp"
},
{
  "directory": "${PROJECT_PATH}/DBx1000/build",
  "command": "clang++  -DNOGRAPHITE=1 ${EXTRA_FLAGS} -I${PROJECT_PATH}/DBx1000 -I${PROJECT_PATH}/DBx1000/benchmarks -I${PROJECT_PATH}/DBx1000/concurrency_control -I${PROJECT_PATH}/DBx1000/storage -I${PROJECT_PATH}/DBx1000/system  -pthread -std=c++11 -Wno-deprecated-declarations   -o ${BUILD_DIR}/concurrency_control/row_vll.cpp.o -c ${PROJECT_PATH}/DBx1000/concurrency_control/row_vll.cpp",
  "file": "${PROJECT_PATH}/DBx1000/concurrency_control/row_vll.cpp"
},
{
  "directory": "${PROJECT_PATH}/DBx1000/build",
  "command": "clang++  -DNOGRAPHITE=1 ${EXTRA_FLAGS} -I${PROJECT_PATH}/DBx1000 -I${PROJECT_PATH}/DBx1000/benchmarks -I${PROJECT_PATH}/DBx1000/concurrency_control -I${PROJECT_PATH}/DBx1000/storage -I${PROJECT_PATH}/DBx1000/system  -pthread -std=c++11 -Wno-deprecated-declarations   -o ${BUILD_DIR}/concurrency_control/vll.cpp.o -c ${PROJECT_PATH}/DBx1000/concurrency_control/vll.cpp",
  "file": "${PROJECT_PATH}/DBx1000/concurrency_control/vll.cpp"
},
{
  "directory": "${PROJECT_PATH}/DBx1000/build",
  "command": "clang++  -DNOGRAPHITE=1 ${EXTRA_FLAGS} -I${PROJECT_PATH}/DBx1000 -I${PROJECT_PATH}/DBx1000/benchmarks -I${PROJECT_PATH}/DBx1000/concurrency_control -I${PROJECT_PATH}/DBx1000/storage -I${PROJECT_PATH}/DBx1000/system  -pthread -std=c++11 -Wno-deprecated-declarations   -o ${BUILD_DIR}/storage/catalog.cpp.o -c ${PROJECT_PATH}/DBx1000/storage/catalog.cpp",
  "file": "${PROJECT_PATH}/DBx1000/storage/catalog.cpp"
},
{
  "directory": "${PROJECT_PATH}/DBx1000/build",
  "command": "clang++  -DNOGRAPHITE=1 ${EXTRA_FLAGS} -I${PROJECT_PATH}/DBx1000 -I${PROJECT_PATH}/DBx1000/benchmarks -I${PROJECT_PATH}/DBx1000/concurrency_control -I${PROJECT_PATH}/DBx1000/storage -I${PROJECT_PATH}/DBx1000/system  -pthread -std=c++11 -Wno-deprecated-declarations   -o ${BUILD_DIR}/storage/index_btree.cpp.o -c ${PROJECT_PATH}/DBx1000/storage/index_btree.cpp",
  "file": "${PROJECT_PATH}/DBx1000/storage/index_btree.cpp"
},
{
  "directory": "${PROJECT_PATH}/DBx1000/build",
  "command": "clang++  -DNOGRAPHITE=1 ${EXTRA_FLAGS} -I${PROJECT_PATH}/DBx1000 -I${PROJECT_PATH}/DBx1000/benchmarks -I${PROJECT_PATH}/DBx1000/concurrency_control -I${PROJECT_PATH}/DBx1000/storage -I${PROJECT_PATH}/DBx1000/system  -pthread -std=c++11 -Wno-deprecated-declarations   -o ${BUILD_DIR}/storage/index_hash.cpp.o -c ${PROJECT_PATH}/DBx1000/storage/index_hash.cpp",
  "file": "${PROJECT_PATH}/DBx1000/storage/index_hash.cpp"
},
{
  "directory": "${PROJECT_PATH}/DBx1000/build",
  "command": "clang++  -DNOGRAPHITE=1 ${EXTRA_FLAGS} -I${PROJECT_PATH}/DBx1000 -I${PROJECT_PATH}/DBx1000/benchmarks -I${PROJECT_PATH}/DBx1000/concurrency_control -I${PROJECT_PATH}/DBx1000/storage -I${PROJECT_PATH}/DBx1000/system  -pthread -std=c++11 -Wno-deprecated-declarations   -o ${BUILD_DIR}/storage/row.cpp.o -c ${PROJECT_PATH}/DBx1000/storage/row.cpp",
  "file": "${PROJECT_PATH}/DBx1000/storage/row.cpp"
},
{
  "directory": "${PROJECT_PATH}/DBx1000/build",
  "command": "clang++  -DNOGRAPHITE=1 ${EXTRA_FLAGS} -I${PROJECT_PATH}/DBx1000 -I${PROJECT_PATH}/DBx1000/benchmarks -I${PROJECT_PATH}/DBx1000/concurrency_control -I${PROJECT_PATH}/DBx1000/storage -I${PROJECT_PATH}/DBx1000/system  -pthread -std=c++11 -Wno-deprecated-declarations   -o ${BUILD_DIR}/storage/table.cpp.o -c ${PROJECT_PATH}/DBx1000/storage/table.cpp",
  "file": "${PROJECT_PATH}/DBx1000/storage/table.cpp"
},
{
  "directory": "${PROJECT_PATH}/DBx1000/build",
  "command": "clang++  -DNOGRAPHITE=1 ${EXTRA_FLAGS} -I${PROJECT_PATH}/DBx1000 -I${PROJECT_PATH}/DBx1000/benchmarks -I${PROJECT_PATH}/DBx1000/concurrency_control -I${PROJECT_PATH}/DBx1000/storage -I${PROJECT_PATH}/DBx1000/system  -pthread -std=c++11 -Wno-deprecated-declarations   -o ${BUILD_DIR}/system/global.cpp.o -c ${PROJECT_PATH}/DBx1000/system/global.cpp",
  "file": "${PROJECT_PATH}/DBx1000/system/global.cpp"
},
{
  "directory": "${PROJECT_PATH}/DBx1000/build",
  "command": "clang++  -DNOGRAPHITE=1 ${EXTRA_FLAGS} -I${PROJECT_PATH}/DBx1000 -I${PROJECT_PATH}/DBx1000/benchmarks -I${PROJECT_PATH}/DBx1000/concurrency_control -I${PROJECT_PATH}/DBx1000/storage -I${PROJECT_PATH}/DBx1000/system  -pthread -std=c++11 -Wno-deprecated-declarations   -o ${BUILD_DIR}/system/helper.cpp.o -c ${PROJECT_PATH}/DBx1000/system/helper.cpp",
  "file": "${PROJECT_PATH}/DBx1000/system/helper.cpp"
},
{
  "directory": "${PROJECT_PATH}/DBx1000/build",
  "command": "clang++  -DNOGRAPHITE=1 ${EXTRA_FLAGS} -I${PROJECT_PATH}/DBx1000 -I${PROJECT_PATH}/DBx1000/benchmarks -I${PROJECT_PATH}/DBx1000/concurrency_control -I${PROJECT_PATH}/DBx1000/storage -I${PROJECT_PATH}/DBx1000/system  -pthread -std=c++11 -Wno-deprecated-declarations   -o ${BUILD_DIR}/system/main.cpp.o -c ${PROJECT_PATH}/DBx1000/system/main.cpp",
  "file": "${PROJECT_PATH}/DBx1000/system/main.cpp"
},
{
  "directory": "${PROJECT_PATH}/DBx1000/build",
  "command": "clang++  -DNOGRAPHITE=1 ${EXTRA_FLAGS} -I${PROJECT_PATH}/DBx1000 -I${PROJECT_PATH}/DBx1000/benchmarks -I${PROJECT_PATH}/DBx1000/concurrency_control -I${PROJECT_PATH}/DBx1000/storage -I${PROJECT_PATH}/DBx1000/system  -pthread -std=c++11 -Wno-deprecated-declarations   -o ${BUILD_DIR}/system/manager.cpp.o -c ${PROJECT_PATH}/DBx1000/system/manager.cpp",
  "file": "${PROJECT_PATH}/DBx1000/system/manager.cpp"
},
{
  "directory": "${PROJECT_PATH}/DBx1000/build",
  "command": "clang++  -DNOGRAPHITE=1 ${EXTRA_FLAGS} -I${PROJECT_PATH}/DBx1000 -I${PROJECT_PATH}/DBx1000/benchmarks -I${PROJECT_PATH}/DBx1000/concurrency_control -I${PROJECT_PATH}/DBx1000/storage -I${PROJECT_PATH}/DBx1000/system  -pthread -std=c++11 -Wno-deprecated-declarations   -o ${BUILD_DIR}/system/mem_alloc.cpp.o -c ${PROJECT_PATH}/DBx1000/system/mem_alloc.cpp",
  "file": "${PROJECT_PATH}/DBx1000/system/mem_alloc.cpp"
},
{
  "directory": "${PROJECT_PATH}/DBx1000/build",
  "command": "clang++  -DNOGRAPHITE=1 ${EXTRA_FLAGS} -I${PROJECT_PATH}/DBx1000 -I${PROJECT_PATH}/DBx1000/benchmarks -I${PROJECT_PATH}/DBx1000/concurrency_control -I${PROJECT_PATH}/DBx1000/storage -I${PROJECT_PATH}/DBx1000/system  -pthread -std=c++11 -Wno-deprecated-declarations   -o ${BUILD_DIR}/system/parser.cpp.o -c ${PROJECT_PATH}/DBx1000/system/parser.cpp",
  "file": "${PROJECT_PATH}/DBx1000/system/parser.cpp"
},
{
  "directory": "${PROJECT_PATH}/DBx1000/build",
  "command": "clang++  -DNOGRAPHITE=1 ${EXTRA_FLAGS} -I${PROJECT_PATH}/DBx1000 -I${PROJECT_PATH}/DBx1000/benchmarks -I${PROJECT_PATH}/DBx1000/concurrency_control -I${PROJECT_PATH}/DBx1000/storage -I${PROJECT_PATH}/DBx1000/system  -pthread -std=c++11 -Wno-deprecated-declarations   -o ${BUILD_DIR}/system/query.cpp.o -c ${PROJECT_PATH}/DBx1000/system/query.cpp",
  "file": "${PROJECT_PATH}/DBx1000/system/query.cpp"
},
{
  "directory": "${PROJECT_PATH}/DBx1000/build",
  "command": "clang++  -DNOGRAPHITE=1 ${EXTRA_FLAGS} -I${PROJECT_PATH}/DBx1000 -I${PROJECT_PATH}/DBx1000/benchmarks -I${PROJECT_PATH}/DBx1000/concurrency_control -I${PROJECT_PATH}/DBx1000/storage -I${PROJECT_PATH}/DBx1000/system  -pthread -std=c++11 -Wno-deprecated-declarations   -o ${BUILD_DIR}/system/stats.cpp.o -c ${PROJECT_PATH}/DBx1000/system/stats.cpp",
  "file": "${PROJECT_PATH}/DBx1000/system/stats.cpp"
},
{
  "directory": "${PROJECT_PATH}/DBx1000/build",
  "command": "clang++  -DNOGRAPHITE=1 ${EXTRA_FLAGS} -I${PROJECT_PATH}/DBx1000 -I${PROJECT_PATH}/DBx1000/benchmarks -I${PROJECT_PATH}/DBx1000/concurrency_control -I${PROJECT_PATH}/DBx1000/storage -I${PROJECT_PATH}/DBx1000/system  -pthread -std=c++11 -Wno-deprecated-declarations   -o ${BUILD_DIR}/system/thread.cpp.o -c ${PROJECT_PATH}/DBx1000/system/thread.cpp",
  "file": "${PROJECT_PATH}/DBx1000/system/thread.cpp"
},
{
  "directory": "${PROJECT_PATH}/DBx1000/build",
  "command": "clang++  -DNOGRAPHITE=1 ${EXTRA_FLAGS} -I${PROJECT_PATH}/DBx1000 -I${PROJECT_PATH}/DBx1000/benchmarks -I${PROJECT_PATH}/DBx1000/concurrency_control -I${PROJECT_PATH}/DBx1000/storage -I${PROJECT_PATH}/DBx1000/system  -pthread -std=c++11 -Wno-deprecated-declarations   -o ${BUILD_DIR}/system/txn.cpp.o -c ${PROJECT_PATH}/DBx1000/system/txn.cpp",
  "file": "${PROJECT_PATH}/DBx1000/system/txn.cpp"
},
{
  "directory": "${PROJECT_PATH}/DBx1000/build",
  "command": "clang++  -DNOGRAPHITE=1 ${EXTRA_FLAGS} -I${PROJECT_PATH}/DBx1000 -I${PROJECT_PATH}/DBx1000/benchmarks -I${PROJECT_PATH}/DBx1000/concurrency_control -I${PROJECT_PATH}/DBx1000/storage -I${PROJECT_PATH}/DBx1000/system  -pthread -std=c++11 -Wno-deprecated-declarations   -o ${BUILD_DIR}/system/wl.cpp.o -c ${PROJECT_PATH}/DBx1000/system/wl.cpp",
  "file": "${PROJECT_PATH}/DBx1000/system/wl.cpp"
},
{
  "directory": "${PROJECT_PATH}/DBx1000/build",
  "command": "clang++  -DNOGRAPHITE=1 ${EXTRA_FLAGS} -I${PROJECT_PATH}/DBx1000 -I${PROJECT_PATH}/DBx1000/benchmarks -I${PROJECT_PATH}/DBx1000/concurrency_control -I${PROJECT_PATH}/DBx1000/storage -I${PROJECT_PATH}/DBx1000/system  -pthread -std=c++11 -Wno-deprecated-declarations   -o ${BUILD_DIR}/config.cpp.o -c ${PROJECT_PATH}/DBx1000/config.cpp",
  "file": "${PROJECT_PATH}/DBx1000/config.cpp"
}
]
EOF
