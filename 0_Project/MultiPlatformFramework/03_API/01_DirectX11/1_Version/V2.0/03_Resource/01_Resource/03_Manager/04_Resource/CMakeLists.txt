# ===========================================
# DirectX11_ResourceManager の	CMakeLists.txt
# ===========================================

# 階層を取得
set(DX11MG_RESOURCE_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# インクルードディレクトリーに追加
target_include_directories(API_Lib PRIVATE "${DX11MG_RESOURCE_ROOT}/Include_Private")

# ソースファイルを追加
target_sources(API_Lib PRIVATE
	"${DX11MG_RESOURCE_ROOT}/Src/DirectX11_ResourceManager.cpp")
