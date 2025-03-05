cmake_minimum_required(VERSION 3.10)
project(sophpi)

# 设置C standard to use
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# 使用静态链接
set(CMAKE_EXE_LINKER_FLAGS "-static")

# 添加宏定义
add_definitions(-D__CV181X__)

# 添加头文件目录
include_directories(include)
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/../cvi_mpi/include)

# 添加库文件路径
link_directories(${CMAKE_SOURCE_DIR}/../cvi_mpi/lib)

# 定义源文件目录
set(SOURCE_DIR src)

# 查找所有 .c 文件
file(GLOB_RECURSE SOURCE_FILES "${SOURCE_DIR}/*.c")

# 创建可执行文件
add_executable(app ${SOURCE_FILES})

# 寻找静态库
find_library(LIBSYS libsys.a PATHS ${CMAKE_SOURCE_DIR}/../cvi_mpi/lib)
find_library(LIBATOMIC libatomic.a PATHS ${CMAKE_SOURCE_DIR}/../host-tools/gcc/riscv64-linux-musl-x86_64/sysroot/lib64v0p7_xthead/lp64d/)
find_library(LIBVI libvi.a PATHS ${CMAKE_SOURCE_DIR}/../cvi_mpi/lib)
find_library(LIBISP libisp.a PATHS ${CMAKE_SOURCE_DIR}/../cvi_mpi/lib)
find_library(LIBISP_ALGO libisp_algo.a PATHS ${CMAKE_SOURCE_DIR}/../cvi_mpi/lib)
find_library(LIBAE libae.a PATHS ${CMAKE_SOURCE_DIR}/../cvi_mpi/lib)
find_library(LIBAWB libawb.a PATHS ${CMAKE_SOURCE_DIR}/../cvi_mpi/lib)

# 链接静态库
target_link_libraries(app ${LIBVI} ${LIBISP} ${LIBAWB} ${LIBISP_ALGO} ${LIBAE} ${LIBSYS} ${LIBATOMIC})
