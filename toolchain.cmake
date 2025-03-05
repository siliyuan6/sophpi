# 设置交叉编译的目标架构
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR riscv)

# 设置交叉编译工具链的路径
set(CMAKE_C_COMPILER /home/liyuan/code/sophgo/sophpi/host-tools/gcc/riscv64-linux-musl-x86_64/bin/riscv64-unknown-linux-musl-gcc)
set(CMAKE_CXX_COMPILER /home/liyuan/code/sophgo/sophpi/host-tools/gcc/riscv64-linux-musl-x86_64/bin/riscv64-unknown-linux-musl-g++)

