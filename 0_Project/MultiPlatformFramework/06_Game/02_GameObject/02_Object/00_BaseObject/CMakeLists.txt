# ==================================================
# BaseObject の CMakeLists.txt
# ==================================================

# 現在の階層を取得
set(BaseObject_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# ヘッダー追加
target_include_directories(Game_Lib PRIVATE "${BaseObject_ROOT}/Include_Private")
