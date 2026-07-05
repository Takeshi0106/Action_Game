# =============================================
# Interface_OSSystem 用の CMakeLists.txt
# =============================================

set(Interface_OSSystem_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# インクルードディレクトリーに追加
target_include_directories(Core_Lib PUBLIC "${Interface_OSSystem_ROOT}/Include_Public")
# ソースファイルを追加
target_sources(Core_Lib PRIVATE "${Interface_OSSystem_ROOT}/Src/IPlatformSystem.cpp")
