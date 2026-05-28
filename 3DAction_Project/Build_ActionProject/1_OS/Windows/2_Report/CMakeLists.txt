# =================================================
# Report 用の CMakeLists.txt
# =================================================

# 関数定義
function(Add_WIN_Report TARGET)
	# 階層を取得
	set(WIN_REPORT_ROOT ${CMAKE_CURRENT_FUNCTION_LIST_DIR})
	
	# ソースを追加
	target_sources(${TARGET} PRIVATE
		"${WIN_REPORT_ROOT}/Src/Windows_ReportMessage.cpp")
endfunction()
