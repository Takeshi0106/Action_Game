# ==================================================
# Math の CMakeLists.txt
# ==================================================

# 現在の階層を取得
set(MATH_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# インクルードディレクトリーに追加
target_include_directories(Core_Lib PUBLIC "${MATH_ROOT}/Include_Public")
add_subdirectory(Src)
