# ==================================================
# Binary の CMakeLists.txt
# ==================================================

# 関数定義
function(Add_Binary TARGET)
	# 現在の階層を取得
	set(BINARY_ROOT ${CMAKE_CURRENT_FUNCTION_LIST_DIR})

	# インクルードディレクトリーに追加
	target_include_directories(${TARGET} PRIVATE "${BINARY_ROOT}/Include")
endfunction()
