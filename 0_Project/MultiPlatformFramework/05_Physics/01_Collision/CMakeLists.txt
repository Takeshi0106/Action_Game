# ==================================================
# Collision の CMakeLists.txt
# ==================================================

# 現在の階層を取得
set(Collision_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# インクルードディレクトリーに追加
target_include_directories(Physics_Lib PUBLIC "${Collision_ROOT}/Include_Public")
