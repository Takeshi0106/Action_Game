# =========================================
# Timer の CMakeLists.txt
# =========================================

# 現在の階層を取得
set(TIMER_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# インクルードディレクトリーに追加
target_include_directories(Core_Lib PUBLIC "${TIMER_ROOT}/Include_Public")
# Srcを追加
add_subdirectory(Src)
