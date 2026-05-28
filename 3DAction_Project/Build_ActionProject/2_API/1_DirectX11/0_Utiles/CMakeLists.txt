# ===========================================
# DirectX1Utiles の	CMakeLists.txt
# ===========================================

# --------------------------------------
# 関数
# --------------------------------------
function(Add_DX11Utiles TARGET)
	# 階層を取得
	set(DX11_UTILES_ROOT ${CMAKE_CURRENT_FUNCTION_LIST_DIR})
	
	# インクルードディレクトリーに追加
	target_include_directories(${TARGET} PRIVATE "${DX11_UTILES_ROOT}/Include")
	
	# ソースファイルを追加
	target_sources(${TARGET} PRIVATE
		"${DX11_UTILES_ROOT}/Src/DirectX11_FormatConverter.cpp")

endfunction()
