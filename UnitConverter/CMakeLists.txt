# ============================================================
# UnitConverter 根构建脚本
# 项目级设置，并把 unitlib 与主程序组装起来
# ============================================================

cmake_minimum_required(VERSION 3.10)

project(UnitConverter VERSION 1.0.0 LANGUAGES CXX)

# 统一使用 C++11，并不允许编译器加 GNU 扩展
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 不指定 -DCMAKE_BUILD_TYPE 时默认 Release（开启优化）
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "构建类型" FORCE)
endif()

# 先定义库目标 unitlib，再定义依赖它的可执行文件
add_subdirectory(unitlib)

add_executable(unitconverter src/main.cpp)

target_link_libraries(unitconverter PRIVATE unitlib)
