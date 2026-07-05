# ==================================================
# FadeConfig の CMakeLists.txt
# ==================================================

# 現在の階層を取得
set(FadeConfig_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# ヘッダー追加
target_include_directories(Game_Lib PUBLIC "${FadeConfig_ROOT}/Include_Public")
