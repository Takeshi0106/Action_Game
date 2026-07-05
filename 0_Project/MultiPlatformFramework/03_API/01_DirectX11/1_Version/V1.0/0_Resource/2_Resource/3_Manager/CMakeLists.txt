# ========================================
# V1.0 APIManager の CMakeLists.txt
# ========================================

# 階層を取得
set(DX11_API_MANAGER_V1_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# 公開するインクルードディレクトリーに追加
target_include_directories(API_Lib PUBLIC "${DX11_API_MANAGER_V1_ROOT}/Include_Public")

# ソースファイルを追加
target_sources(API_Lib PRIVATE
	"${DX11_API_MANAGER_V1_ROOT}/Src/VertexBufferManager.cpp"
	"${DX11_API_MANAGER_V1_ROOT}/Src/IndexBufferManager.cpp"
	
	"${DX11_API_MANAGER_V1_ROOT}/Src/ConstantBufferManager.cpp"

	"${DX11_API_MANAGER_V1_ROOT}/Src/ShaderManager.cpp"
	"${DX11_API_MANAGER_V1_ROOT}/Src/SamplerManager.cpp"

	"${DX11_API_MANAGER_V1_ROOT}/Src/TextureManager.cpp"
	"${DX11_API_MANAGER_V1_ROOT}/Src/ResourceViewManager.cpp"

	"${DX11_API_MANAGER_V1_ROOT}/Src/DirectX11_DrawManager.cpp")
