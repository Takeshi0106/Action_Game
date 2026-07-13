# ===========================================
# Common 用の CMakeLists.txt
# ===========================================

set(WindowsCommon_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# インクルードディレクトリーに追加
target_include_directories(OS_Lib PUBLIC "${WindowsCommon_ROOT}/Include_Public")
