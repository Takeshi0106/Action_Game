# =================================================
# Timer 用の CMakeLists.txt
# =================================================

# 関数定義
function(Add_WIN_Timer TARGET)

	# 階層を取得
	set(WIN_TIMER_ROOT ${CMAKE_CURRENT_FUNCTION_LIST_DIR})
	
	# ソースを追加
	target_sources(${TARGET} PRIVATE
		"${WIN_TIMER_ROOT}/Src/Windows_Timer.cpp")
endfunction()
