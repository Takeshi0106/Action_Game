# =========================================
# Timer の CMakeLists.txt
# =========================================

# 関数定義
function(Add_Timer TARGET)
	# 現在の階層を取得
	set(TIMER_ROOT ${CMAKE_CURRENT_FUNCTION_LIST_DIR})

	# インクルードディレクトリーに追加
	target_include_directories(${TARGET} PRIVATE "${TIMER_ROOT}/Include")
endfunction()
