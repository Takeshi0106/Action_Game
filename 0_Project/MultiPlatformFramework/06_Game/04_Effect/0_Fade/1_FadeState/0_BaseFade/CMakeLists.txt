# ==================================================
# BaseFade の CMakeLists.txt
# ==================================================

# 現在の階層を取得
set(BaseFade_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# ヘッダー追加
target_include_directories(Game_Lib PUBLIC "${BaseFade_ROOT}/Include_Public")
