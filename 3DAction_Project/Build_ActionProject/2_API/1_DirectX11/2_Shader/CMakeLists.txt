# ==================================================
# DirectX11APIShader の CMakeLists.txt
# ==================================================

# --------------------------------------
# 関数
# --------------------------------------
function(Add_DX11_Shader TARGET)

	# 階層を取得
	set(DX11_Shader_ROOT ${CMAKE_CURRENT_FUNCTION_LIST_DIR})
	
	# デバッグビルドの場合のみ追加
	if(CMAKE_BUILD_TYPE STREQUAL "Debug")
		# インクルードディレクトリーに追加
		target_include_directories(${TARGET} PRIVATE "${DX11_Shader_ROOT}/Include")
		
		# ソースファイルを追加
		target_sources(${TARGET} PRIVATE
			"${DX11_Shader_ROOT}/Src/DirectX11_DXBCCompileModule.cpp"
			"${DX11_Shader_ROOT}/Src/DirectX11_DevelopmentCompileModule.cpp"
		)
	endif()

endfunction()
