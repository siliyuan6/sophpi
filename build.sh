#!/bin/bash

# 设置项目根目录
PROJECT_DIR=$(pwd)

# 设置构建目录
BUILD_DIR="$PROJECT_DIR/build"

# 设置工具链文件路径（假设工具链文件在项目根目录下）
TOOLCHAIN_FILE="$PROJECT_DIR/toolchain.cmake"

# 检查构建目录是否存在，如果存在则清空它
if [ -d "$BUILD_DIR" ]; then
    echo "[build.sh] Cleaning up old build directory..."
    rm -rf "$BUILD_DIR"
fi

# 创建一个新的构建目录
mkdir -p "$BUILD_DIR"

# 进入构建目录
cd "$BUILD_DIR"

# 运行 CMake 配置，指定工具链文件
echo "[build.sh] Running cmake..."
cmake .. -DCMAKE_TOOLCHAIN_FILE="$TOOLCHAIN_FILE"

# 编译项目
echo "[build.sh] Building project..."
make

# 完成构建
echo "[build.sh] Build complete!"
cp app ../
