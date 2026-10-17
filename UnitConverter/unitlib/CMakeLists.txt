# ============================================================
# unitlib 子构建脚本
# 产物：静态库 libunitlib.a
# ============================================================

add_library(unitlib STATIC src/unitlib.cpp)

target_include_directories(unitlib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
