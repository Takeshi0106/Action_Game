# ==================================================
# API_Resource の CMakeLists.txt
# ==================================================

# 階層を取得
set(API_IRESOURCE_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

message(STATUS "API_IRESOURCE_ROOT: ${API_IRESOURCE_ROOT}")

# インクルードディレクトリーに追加
target_include_directories(API_Lib PUBLIC "${API_IRESOURCE_ROOT}/Include_Public")
