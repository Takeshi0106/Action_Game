# ==================================================
# Game の CMakeLists.txt
# ==================================================

# ライブラリ作成
add_library(Game_Lib STATIC)

# ライブラリを追加
target_link_libraries(Game_Lib INTERFACE Core_Lib)
target_link_libraries(Game_Lib INTERFACE Physics_Lib)

# モジュールの各機能を追加
add_subdirectory(0_Collider)
add_subdirectory(1_Collision)
