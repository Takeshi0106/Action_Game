# ===========================================
# DirectX11Loader の	CMakeLists.txt
# ===========================================

# --------------------------------------
# 関数
# --------------------------------------
function(Add_DX11Loader TARGET)
	# 階層を取得
	set(DX11_LOADER_ROOT ${CMAKE_CURRENT_FUNCTION_LIST_DIR})
	
	# インクルードディレクトリーに追加
	target_include_directories(${TARGET} PRIVATE "${DX11_LOADER_ROOT}/Include")
	
	# ソースファイルを追加
	target_sources(${TARGET} PRIVATE
		"${DX11_LOADER_ROOT}/Src/DirectX11_TextureLoadModule.cpp")

endfunction()
