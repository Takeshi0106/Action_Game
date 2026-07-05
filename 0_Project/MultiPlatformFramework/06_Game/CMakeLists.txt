# ==================================================
# Game の CMakeLists.txt
# ==================================================

# ライブラリ作成
add_library(Game_Lib STATIC)

# ライブラリを追加
target_link_libraries(Game_Lib PRIVATE Core_Lib)
target_link_libraries(Game_Lib PRIVATE $<$<CONFIG:Debug>:IMGUI>)
target_link_libraries(Game_Lib PRIVATE Physics_Lib)

# モジュールの各機能を追加
add_subdirectory(00_Controller)
add_subdirectory(01_DrawModule)
add_subdirectory(02_GameObject)
add_subdirectory(03_GameScene)
add_subdirectory(04_Effect)
