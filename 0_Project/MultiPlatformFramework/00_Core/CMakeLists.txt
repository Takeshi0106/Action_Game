# =============================================
# Core 用の CMakeLists.txt
# =============================================
# Core ライブラリを作成
add_library(Core_Lib STATIC)

# サブディレクトリーを追加
add_subdirectory(00_Binary)
add_subdirectory(01_Math)
add_subdirectory(02_String)
add_subdirectory(03_Timer)
add_subdirectory(04_Report)
add_subdirectory(05_File)
add_subdirectory(06_Interface)
add_subdirectory(07_Template)
add_subdirectory(08_Utiles)
add_subdirectory(09_Handle)
add_subdirectory(10_Material)
add_subdirectory(11_Config)

# 念のため、デファイン追加
target_compile_definitions(Core_Lib PUBLIC
    $<$<CONFIG:Debug>:_DEBUG>
    $<$<CONFIG:Release>:NDEBUG>)
