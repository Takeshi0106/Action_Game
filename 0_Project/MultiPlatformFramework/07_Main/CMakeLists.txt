# ==================================================
# Main の CMakeLists.txt
# ==================================================

# 現在の階層を取得
set(Main_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# ライブラリを追加
target_link_libraries(${PROJECT_NAME} PRIVATE Core_Lib)
target_link_libraries(${PROJECT_NAME} PRIVATE OS_Lib)
target_link_libraries(${PROJECT_NAME} PRIVATE API_Lib)
target_link_libraries(${PROJECT_NAME} PRIVATE $<$<CONFIG:Debug>:IMGUI>)
target_link_libraries(${PROJECT_NAME} PRIVATE Physics_Lib)
target_link_libraries(${PROJECT_NAME} PRIVATE Game_Lib)

# モジュールの各機能を追加
add_subdirectory(00_EntryPoint)
add_subdirectory(01_Game)
