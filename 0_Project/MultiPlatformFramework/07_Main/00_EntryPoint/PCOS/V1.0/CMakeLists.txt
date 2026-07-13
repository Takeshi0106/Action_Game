# ==================================================
# PCOS main の CMakeLists.txt
# ==================================================

# 現在の階層を取得
set(Main_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# Mainを追加
target_sources(${PROJECT_NAME} PRIVATE "${Main_ROOT}/Src/Main.cpp")
