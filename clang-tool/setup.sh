set -x
apt-get update
apt-get install software-properties-common git wget -y
wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add -
apt-add-repository "deb http://apt.llvm.org/bionic/ llvm-toolchain-bionic-11 main"
apt-get update
apt-get install -y cmake zsh g++ python3-pip
pip3 install -r ../DBx1000/requirements.txt
apt-get install -y clang-11 clang-tools-11 clang-11-doc libclang-common-11-dev libclang-11-dev libclang1-11 clang-format-11 clangd-11
apt-get install -y libllvm11 llvm-11 llvm-11-dev llvm-11-doc llvm-11-examples llvm-11-runtime
update-alternatives --install /usr/bin/clang clang /usr/bin/clang-11 20
update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-11 20
update-alternatives --install /usr/bin/llvm-config llvm-config /usr/bin/llvm-config-11 20
sh -c "$(wget -O- https://raw.githubusercontent.com/ohmyzsh/ohmyzsh/master/tools/install.sh)"

cd .. 
pwd
export PROJECT_PATH=$(pwd) 
cd DBx1000 
pwd
export BUILD_DIR=$(pwd) 
cd ../clang-tool
echo $PROJECT_PATH
sh -c ./gen_compile_commands.sh > ../compile_commands.json

