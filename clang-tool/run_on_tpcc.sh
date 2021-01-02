PATH=".."
set -x
#./gen_compile_commands.sh > ../compile_commands.json
cmake-build-debug/bin/clang-tool ../DBx1000/*/*.cpp
