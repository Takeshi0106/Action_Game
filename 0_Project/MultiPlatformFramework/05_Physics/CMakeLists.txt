# ==================================================
# Physics(当たり判定などの物理処理) の CMakeLists.txt
# ==================================================

# ライブラリ作成
add_library(Physics_Lib STATIC)

# ライブラリを追加
target_link_libraries(Physics_Lib PRIVATE Core_Lib)

# モジュールの各機能を追加
add_subdirectory(00_Collider)
add_subdirectory(01_Collision)
add_subdirectory(02_BroadPhase)
add_subdirectory(03_Module)
