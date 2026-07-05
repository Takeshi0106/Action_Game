# ===========================================
# DirectX1Utiles の	CMakeLists.txt
# ===========================================

# 階層を取得
set(DX11_UTILES_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# インクルードディレクトリーに追加
target_include_directories(API_Lib PRIVATE "${DX11_UTILES_ROOT}/Include_Private")

# ソースファイルを追加
target_sources(API_Lib PRIVATE
	"${DX11_UTILES_ROOT}/Src/DirectX11_FormatConverter.cpp")
