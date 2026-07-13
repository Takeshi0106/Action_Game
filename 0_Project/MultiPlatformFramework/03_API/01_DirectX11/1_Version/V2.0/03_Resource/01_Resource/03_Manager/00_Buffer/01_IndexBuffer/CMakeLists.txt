# ===========================================
# DirectX11_IndexBufferManager の CMakeLists.txt
# ===========================================

# 階層を取得
set(DX11MG_IB_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# インクルードディレクトリーに追加
target_include_directories(API_Lib PRIVATE "${DX11MG_IB_ROOT}/Include_Private")

# ソースファイルを追加
target_sources(API_Lib PRIVATE
    "${DX11MG_IB_ROOT}/Src/DirectX11_IndexBufferManager.cpp")
