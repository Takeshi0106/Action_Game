# ===========================================
# DirectX11Factory の	CMakeLists.txt
# ===========================================

# --------------------------------------
# 関数
# --------------------------------------
function(Add_DX11Factory TARGET)

	# 階層を取得
	set(DX11_FACTORY_ROOT ${CMAKE_CURRENT_FUNCTION_LIST_DIR})
	
	# インクルードディレクトリーに追加
	target_include_directories(${TARGET} PRIVATE "${DX11_FACTORY_ROOT}/Include")
	
	# ソースファイルを追加
	target_sources(${TARGET} PRIVATE
		"${DX11_FACTORY_ROOT}/Src/DirectX11_ResourceFactory.cpp"
	)

endfunction()
