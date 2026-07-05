# ==================================================
# DirectX11Struct の CMakeLists.txt
# ==================================================

# 階層を取得
set(DX11_API_STRUCT_V2_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# インクルードディレクトリーに追加
target_include_directories(API_Lib PRIVATE "${DX11_API_STRUCT_V2_ROOT}/Include_Private")
