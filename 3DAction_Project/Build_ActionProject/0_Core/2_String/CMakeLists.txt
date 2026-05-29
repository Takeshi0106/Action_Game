# ============================================
# String の CMakeLists.txt
# ============================================

# 現在の階層を取得
set(STRING_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# インクルードディレクトリーに追加
target_include_directories(Core_Lib PUBLIC "${STRING_ROOT}/Include")

# ソースファイルを追加
target_sources(Core_Lib PUBLIC
    "${STRING_ROOT}/Src/UTF8_String.cpp")
