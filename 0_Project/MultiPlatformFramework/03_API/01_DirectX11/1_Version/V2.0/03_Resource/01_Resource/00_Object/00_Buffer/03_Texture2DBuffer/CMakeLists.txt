# ===========================================
# DirectX11_Texture2DBuffer の	CMakeLists.txt
# ===========================================

# 階層を取得
set(DX11OBJ_T2B_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# インクルードディレクトリーに追加
target_include_directories(API_Lib PRIVATE "${DX11OBJ_T2B_ROOT}/Include_Private")

# ソースファイルを追加
target_sources(API_Lib PRIVATE
	"${DX11OBJ_T2B_ROOT}/Src/DirectX11_Texture2DBufferData.cpp")
