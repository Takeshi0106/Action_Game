# ===========================================
# DirectX11APIManager の	CMakeLists.txt
# ===========================================

# --------------------------------------
# 関数
# --------------------------------------
function(Add_DX11APIManager TARGET)

	# 階層を取得
	set(DX11_API_Manager_ROOT ${CMAKE_CURRENT_FUNCTION_LIST_DIR})
	
	# インクルードディレクトリーに追加
	target_include_directories(${TARGET} PRIVATE "${DX11_API_Manager_ROOT}/Include")
	
	# ソースファイルを追加
	target_sources(${TARGET} PRIVATE
		"${DX11_API_Manager_ROOT}/Src/DirectX11_VertexBufferManager.cpp"
	    "${DX11_API_Manager_ROOT}/Src/DirectX11_IndexBufferManager.cpp"
		"${DX11_API_Manager_ROOT}/Src/DirectX11_ConstantBufferManager.cpp"
		"${DX11_API_Manager_ROOT}/Src/DirectX11_ShaderManager.cpp"

		"${DX11_API_Manager_ROOT}/Src/DirectX11_Texture2DBufferManager.cpp"
		"${DX11_API_Manager_ROOT}/Src/DirectX11_ViewManager.cpp"
		"${DX11_API_Manager_ROOT}/Src/DirectX11_TextureResourceManager.cpp"

		"${DX11_API_Manager_ROOT}/Src/DirectX11_SamplerManager.cpp"

		"${DX11_API_Manager_ROOT}/Src/DirectX11_ResourceManager.cpp")

endfunction()
