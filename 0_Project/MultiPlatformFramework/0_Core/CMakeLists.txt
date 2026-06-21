# =============================================
# Core 用の CMakeLists.txt
# =============================================
# Core ライブラリを作成
add_library(Core_Lib STATIC)

# サブディレクトリーを追加
add_subdirectory(0_Binary)
add_subdirectory(1_Math)
add_subdirectory(2_String)
add_subdirectory(3_Timer)
add_subdirectory(4_Report)
add_subdirectory(5_File)
add_subdirectory(6_Interface)
add_subdirectory(7_Template)
add_subdirectory(8_Utiles)
add_subdirectory(9_Handle)
add_subdirectory(10_Material)

# 念のため、Debugを追加
# IDEでDeubgがグレーアウトするため追加
target_compile_definitions(Core_Lib PUBLIC
    $<$<CONFIG:Debug>:_DEBUG>
    $<$<CONFIG:Release>:NDEBUG>)
