# ==================================================
# File の CMakeLists.txt
# ==================================================

# 現在の階層を取得
set(FileUTils_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# インクルードディレクトリーに追加
target_include_directories(Core_Lib PUBLIC "${FileUTils_ROOT}/Include_Public")

# ソースファイルを追加
add_subdirectory(Src)
