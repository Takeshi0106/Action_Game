# ==================================================
# Collision の CMakeLists.txt
# ==================================================

# 関数定義
function(Add_Collision TARGET)
	# 現在の階層を取得
	set(Collision_ROOT ${CMAKE_CURRENT_FUNCTION_LIST_DIR})

	# インクルードディレクトリーに追加
	target_include_directories(${TARGET} PRIVATE "${Collision_ROOT}/Include")
endfunction()
