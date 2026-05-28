# ==================================================
# Report の CMakeLists.txt
# ==================================================

# 関数定義
function(Add_Report TARGET)
	# 現在の階層を取得
	set(REPORT_ROOT ${CMAKE_CURRENT_FUNCTION_LIST_DIR})

	# インクルードディレクトリーに追加
	target_include_directories(${TARGET} PRIVATE "${REPORT_ROOT}/Include")
endfunction()
