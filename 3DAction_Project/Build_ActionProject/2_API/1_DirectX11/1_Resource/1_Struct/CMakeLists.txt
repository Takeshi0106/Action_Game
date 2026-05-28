# ==================================================
# DirectX11Struct の CMakeLists.txt
# ==================================================

function(Add_DX11Struct TARGET)
	# 階層を取得
	set(DX11_API_STRUCT_ROOT ${CMAKE_CURRENT_FUNCTION_LIST_DIR})
	
	# インクルードディレクトリーに追加
	target_include_directories(${TARGET} PRIVATE "${DX11_API_STRUCT_ROOT}/Include")

endfunction()
