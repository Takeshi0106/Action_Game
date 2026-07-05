# =============================================
# InterfaceGame 用の CMakeLists.txt
# =============================================

# 現在の階層を取得
set(Interface_Game_V1_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# インクルードディレクトリーに追加
target_include_directories(Core_Lib PUBLIC "${Interface_Game_V1_ROOT}/Include_Public")
