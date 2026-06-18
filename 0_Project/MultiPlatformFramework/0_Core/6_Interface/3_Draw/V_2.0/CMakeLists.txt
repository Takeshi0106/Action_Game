# ============================================
# インターフェイスV_2.0 のCMakeLists.txt
# ============================================

# 現在の階層を取得
set(Interface_2_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# インクルードディレクトリーに追加
target_include_directories(Core_Lib PUBLIC "${Interface_2_ROOT}/Include_Public")
