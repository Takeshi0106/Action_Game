# ===========================================
# OS 用の CMakeLists.txt
# ===========================================
# 静的ライブラリーを作成
add_library(OS_Lib STATIC)

# Core ライブラリーをリンク
target_link_libraries(OS_Lib PRIVATE Core_Lib)
# Imgui ライブラリーをリンク Debugビルドのときのみリンクする
target_link_libraries(OS_Lib PRIVATE $<$<CONFIG:Debug>:IMGUI>)

# ===========================================
# CmakeLists.txt を実行
# ===========================================
# 共通
add_subdirectory(00_ShaderReference)

# Windows
if(PLATFORM_OS STREQUAL "WINDOWS")
    add_subdirectory(01_Windows)
endif()

