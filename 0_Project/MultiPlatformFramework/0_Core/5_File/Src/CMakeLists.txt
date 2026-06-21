# ==================================================
# FileSrc の CMakeLists.txt
# ==================================================

# 現在の階層を取得
set(File_SRC_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# ソースファイルを追加
target_sources(Core_Lib PRIVATE
	"${File_SRC_ROOT}/SaveLoadUtiles.cpp")

# プラットフォームごとのSrcを追加
if(PLATFORM_OS STREQUAL "WINDOWS")
	add_subdirectory(0_PCOS)
else()
    message(FATAL_ERROR "Unsupported FileSrc PLATFORM_OS: ${PLATFORM_OS}")
endif()

