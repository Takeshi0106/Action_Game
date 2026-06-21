# ===========================================
# DirectX11APIManager の	CMakeLists.txt
# ===========================================

# 階層を取得
set(DX11_API_Manager_V2_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# インクルードディレクトリーに追加
target_include_directories(API_Lib PRIVATE "${DX11_API_Manager_V2_ROOT}/Include_Private")

# ソースファイルを追加
target_sources(API_Lib PRIVATE
	"${DX11_API_Manager_V2_ROOT}/Src/DirectX11_VertexBufferManager.cpp"
    "${DX11_API_Manager_V2_ROOT}/Src/DirectX11_IndexBufferManager.cpp"
	"${DX11_API_Manager_V2_ROOT}/Src/DirectX11_ConstantBufferManager.cpp"
	"${DX11_API_Manager_V2_ROOT}/Src/DirectX11_ShaderManager.cpp"

	"${DX11_API_Manager_V2_ROOT}/Src/DirectX11_Texture2DBufferManager.cpp"
	"${DX11_API_Manager_V2_ROOT}/Src/DirectX11_ViewManager.cpp"
	"${DX11_API_Manager_V2_ROOT}/Src/DirectX11_TextureResourceManager.cpp"

	"${DX11_API_Manager_V2_ROOT}/Src/DirectX11_SamplerManager.cpp"

	"${DX11_API_Manager_V2_ROOT}/Src/DirectX11_ResourceManager.cpp")
