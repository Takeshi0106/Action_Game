# ==================================================
# BaseScene の CMakeLists.txt
# ==================================================

# 現在の階層を取得
set(BaseScene_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# ヘッダー追加
target_include_directories(Game_Lib PUBLIC "${BaseScene_ROOT}/Include_Public")
