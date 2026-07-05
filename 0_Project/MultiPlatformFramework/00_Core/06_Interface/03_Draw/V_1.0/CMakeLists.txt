# ============================================
# インターフェイスV_1.0 のCMakeLists.txt
# ============================================

# 現在の階層を取得
set(Interface_1_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# インクルードディレクトリーに追加
target_include_directories(Core_Lib PUBLIC "${Interface_1_ROOT}/Include_Public")
