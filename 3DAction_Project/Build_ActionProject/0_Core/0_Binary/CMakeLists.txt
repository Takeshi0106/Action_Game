# ==================================================
# Binary の CMakeLists.txt
# ==================================================

# 現在の階層を取得
set(BINARY_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# インクルードディレクトリーに追加
target_include_directories(Core_Lib PUBLIC "${BINARY_ROOT}/Include")
