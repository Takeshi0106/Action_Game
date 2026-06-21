# ==================================================
# DirectX11APIShader の CMakeLists.txt
# ==================================================

# 階層を取得
set(DX11_Shader_Loader_V1_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# インクルードディレクトリーに追加
target_include_directories(API_Lib PRIVATE "${DX11_Shader_Loader_V1_ROOT}/Include_Private")

# ソースファイルを追加
target_sources(API_Lib PRIVATE
	"${DX11_Shader_Loader_V1_ROOT}/Src/ShaderCompilUtils.cpp"
	"${DX11_Shader_Loader_V1_ROOT}/Src/ShaderReflectionUtils.cpp")
