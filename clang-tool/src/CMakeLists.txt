cmake_minimum_required(VERSION 2.8)

if(UNIX AND NOT APPLE)
        set(LINUX TRUE)
endif()

find_package(LLVM REQUIRED)
find_package(Clang REQUIRED)

set(CMAKE_CXX_FLAGS "-Wall -g3 -O0 -fno-rtti ${LLVM_COMPILE_FLAGS}")

include_directories(${LLVM_INCLUDE_DIRS})
include_directories(${CLANG_INCLUDE_DIRS})


set(CLANG_TOOL_SRC

        main.cc
        
        actions/frontendaction.cc
        consumer/consumer.cc
        utils/utils.cc
        
        transformer/transformer.cc
        transformer/dbspecializer.cc
        
        finder/finder.cc
        finder/dbgraphconstructor.cc
        finder/path_fragment.cpp
        utils/ast_utils.cpp utils/ast_utils.h utils/dbanalysisstorage.cpp utils/dbanalysisstorage.h)

# remove trailing whitespaces
string(STRIP ${LLVM_LDFLAGS} LLVM_LDFLAGS)


add_executable(clang-tool ${CLANG_TOOL_SRC})
if(LINUX)
	target_link_libraries(clang-tool -Wl,--start-group ${CLANG_LIBS} ${LLVM_LIBS_CORE} -Wl,--end-group ${LLVM_LDFLAGS})
else()
	target_link_libraries(clang-tool ${CLANG_LIBS} ${LLVM_LIBS_CORE} ${LLVM_LDFLAGS})
endif()

