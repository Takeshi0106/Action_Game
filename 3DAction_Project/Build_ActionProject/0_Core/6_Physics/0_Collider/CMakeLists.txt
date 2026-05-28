# ==================================================
# Collider の CMakeLists.txt
# ==================================================

# 関数定義
function(Add_Collider TARGET)
	# 現在の階層を取得
	set(Collider_ROOT ${CMAKE_CURRENT_FUNCTION_LIST_DIR})

	# インクルードディレクトリーに追加
	target_include_directories(${TARGET} PRIVATE "${Collider_ROOT}/Include")

endfunction()
