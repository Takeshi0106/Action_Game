# ===========================================
# DirectX11APIObject の	CMakeLists.txt
# ===========================================

# 階層を取得
set(DX11_API_OBJECT_V2_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# インクルードディレクトリーに追加
target_include_directories(API_Lib PRIVATE "${DX11_API_OBJECT_V2_ROOT}/Include_Private")

# ソースファイルを追加
target_sources(API_Lib PRIVATE
	"${DX11_API_OBJECT_V2_ROOT}/Src/DirectX11_VertexBufferData.cpp"
    "${DX11_API_OBJECT_V2_ROOT}/Src/DirectX11_IndexBufferData.cpp"
	"${DX11_API_OBJECT_V2_ROOT}/Src/DirectX11_ConstantBufferData.cpp"
	"${DX11_API_OBJECT_V2_ROOT}/Src/DirectX11_ShaderData.cpp"

	"${DX11_API_OBJECT_V2_ROOT}/Src/DirectX11_Texture2DBufferData.cpp"
	"${DX11_API_OBJECT_V2_ROOT}/Src/DirectX11_ViewData.cpp"
	
	"${DX11_API_OBJECT_V2_ROOT}/Src/DirectX11_SamplerData.cpp")
