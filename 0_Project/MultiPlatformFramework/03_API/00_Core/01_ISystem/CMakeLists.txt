# ==================================================
# API_Resource の CMakeLists.txt
# ==================================================

# 階層を取得
set(API_IRESOURCE_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# インクルードディレクトリーに追加
target_include_directories(API_Lib PRIVATE "${API_IRESOURCE_ROOT}/Include_Private")
