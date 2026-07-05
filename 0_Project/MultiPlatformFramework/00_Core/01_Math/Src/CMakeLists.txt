# ==================================================
# SrcMath の CMakeLists.txt
# どのMathSrcを使用するか判定する
# ==================================================

# プラットフォームごとのSrcを追加
if(PLATFORM_OS STREQUAL "WINDOWS")
	add_subdirectory(0_Windows)
else()
    message(FATAL_ERROR "Unsupported Math PLATFORM_OS: ${PLATFORM_OS}")
endif()
