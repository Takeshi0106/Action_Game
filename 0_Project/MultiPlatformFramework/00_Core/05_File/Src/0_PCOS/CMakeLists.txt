# ==========================================================
# PCOS 用の FileUtils の CMakeLists.txt
# ==========================================================

# 階層を取得
set(PCOS_FILEUTILS_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# ソースファイルを追加
target_sources(Core_Lib PRIVATE
    "${PCOS_FILEUTILS_ROOT}/Src/FileUtils.cpp")
