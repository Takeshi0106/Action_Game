# ==================================================
# DirectX11RendererState の CMakeLists.txt
# ==================================================

# 階層を取得
set(DX11_RENDERER_STATE_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# インクルードディレクトリーに追加
target_include_directories(API_Lib PRIVATE "${DX11_RENDERER_STATE_ROOT}/Include_Private")
# ソースファイルを追加
target_sources(API_Lib PRIVATE
	"${DX11_RENDERER_STATE_ROOT}/Src/DirectX11_RendererStateManager.cpp")
