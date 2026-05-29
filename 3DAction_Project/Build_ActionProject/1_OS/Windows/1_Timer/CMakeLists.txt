# =================================================
# Timer 用の CMakeLists.txt
# =================================================

# 階層を取得
set(WIN_TIMER_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# ソースを追加
target_sources(OS_Lib PRIVATE
	"${WIN_TIMER_ROOT}/Src/Windows_Timer.cpp")
