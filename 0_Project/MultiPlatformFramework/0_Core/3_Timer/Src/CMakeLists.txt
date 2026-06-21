# =========================================
# SrcTimer の CMakeLists.txt
# =========================================

# プラットフォームごとのSrcを追加
if(PLATFORM_OS STREQUAL "WINDOWS")
	add_subdirectory(0_Windows)
else()
    message(FATAL_ERROR "Unsupported Timer PLATFORM_OS: ${PLATFORM_OS}")
endif()
