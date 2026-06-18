# =================================================
# Report 用の CMakeLists.txt
# =================================================

# 階層を取得
set(WINDOWS_REPORT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# ソースを追加
target_sources(Core_Lib PRIVATE
	"${WINDOWS_REPORT_ROOT}/Src/Windows_ReportMessage.cpp")
