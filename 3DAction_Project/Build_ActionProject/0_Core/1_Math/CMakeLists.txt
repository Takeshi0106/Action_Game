# ==================================================
# Math の CMakeLists.txt
# ==================================================

# 関数定義
function(Add_Math TARGET)
	# 現在の階層を取得
	set(MATH_ROOT ${CMAKE_CURRENT_FUNCTION_LIST_DIR})

	# インクルードディレクトリーに追加
	target_include_directories(${TARGET} PRIVATE "${MATH_ROOT}/Include")
endfunction()
