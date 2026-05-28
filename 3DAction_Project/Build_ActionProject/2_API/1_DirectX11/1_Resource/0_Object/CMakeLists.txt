# ===========================================
# DirectX11APIObject の	CMakeLists.txt
# ===========================================

# --------------------------------------
# 関数
# --------------------------------------
function(Add_DX11APIObject TARGET)
	# 階層を取得
	set(DX11_API_OBJECT_ROOT ${CMAKE_CURRENT_FUNCTION_LIST_DIR})
	
	# インクルードディレクトリーに追加
	target_include_directories(${TARGET} PRIVATE "${DX11_API_OBJECT_ROOT}/Include")
	
	# ソースファイルを追加
	target_sources(${TARGET} PRIVATE
		"${DX11_API_OBJECT_ROOT}/Src/DirectX11_VertexBufferData.cpp"
	    "${DX11_API_OBJECT_ROOT}/Src/DirectX11_IndexBufferData.cpp"
		"${DX11_API_OBJECT_ROOT}/Src/DirectX11_ConstantBufferData.cpp"
		"${DX11_API_OBJECT_ROOT}/Src/DirectX11_ShaderData.cpp"

		"${DX11_API_OBJECT_ROOT}/Src/DirectX11_Texture2DBufferData.cpp"
		"${DX11_API_OBJECT_ROOT}/Src/DirectX11_ViewData.cpp"
		
		"${DX11_API_OBJECT_ROOT}/Src/DirectX11_SamplerData.cpp"
	)

endfunction()
