# ========================================
# V1.0 APIObject の CMakeLists.txt
# ========================================

# 階層を取得
set(DX11_API_OBJECT_V1_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# インクルードディレクトリーに追加
target_include_directories(API_Lib PUBLIC "${DX11_API_OBJECT_V1_ROOT}/Include_Public")

# ソースファイルを追加
target_sources(API_Lib PRIVATE
	"${DX11_API_OBJECT_V1_ROOT}/Src/VertexBufferData.cpp"
	"${DX11_API_OBJECT_V1_ROOT}/Src/IndexBufferData.cpp"

	"${DX11_API_OBJECT_V1_ROOT}/Src/ShaderData.cpp"
	"${DX11_API_OBJECT_V1_ROOT}/Src/ConstantBufferData.cpp"

	"${DX11_API_OBJECT_V1_ROOT}/Src/TextureData.cpp"
	"${DX11_API_OBJECT_V1_ROOT}/Src/ResourceViewData.cpp"

	"${DX11_API_OBJECT_V1_ROOT}/Src/SamplerData.cpp")
