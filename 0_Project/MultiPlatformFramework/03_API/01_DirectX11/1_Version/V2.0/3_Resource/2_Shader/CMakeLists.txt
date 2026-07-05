# ==================================================
# DirectX11APIShader の CMakeLists.txt
# ==================================================

# 階層を取得
set(DX11_Shader_V2_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# デバッグビルドの場合のみ追加
if(CMAKE_BUILD_TYPE STREQUAL "Debug")

	# Spirv モジュールを有効にする
	target_link_libraries(API_Lib PRIVATE )

	# インクルードディレクトリーに追加
	target_include_directories(API_Lib PRIVATE "${DX11_Shader_V2_ROOT}/Include")
	
	# ソースファイルを追加
	target_sources(API_Lib PRIVATE
		"${DX11_Shader_V2_ROOT}/Src/DirectX11_DXBCCompileModule.cpp"
		"${DX11_Shader_V2_ROOT}/Src/DirectX11_DevelopmentCompileModule.cpp")

endif()
