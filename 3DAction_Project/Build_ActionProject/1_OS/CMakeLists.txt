# ===========================================
# OS 用の CMakeLists.txt
# ===========================================
# 静的ライブラリーを作成
add_library(OS_Lib STATIC)

# Core ライブラリーをリンク
target_link_libraries(OS_Lib PRIVATE Core_Lib)

# Windows
if(PLATFORM_OS STREQUAL "WINDOWS")
    add_subdirectory(Windows)
    add_subdirectory(0_PCOS)
endif()
