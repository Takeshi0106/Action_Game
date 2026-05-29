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
