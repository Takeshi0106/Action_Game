# ==================================================
# Physics(当たり判定などの物理処理) の CMakeLists.txt
# ==================================================

# ライブラリ作成
add_library(Physics_Lib STATIC)

# ライブラリを追加
target_link_libraries(Physics_Lib PRIVATE Core_Lib OS_Lib)

# モジュールの各機能を追加
add_subdirectory(0_Collider)
add_subdirectory(1_Collision)
