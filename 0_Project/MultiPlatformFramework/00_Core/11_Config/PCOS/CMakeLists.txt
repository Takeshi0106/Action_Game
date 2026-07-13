# =============================================
# Core Config 用の CMakeLists.txt
# =============================================

# 階層を取得
set(Core_Config_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# インクルードディレクトリーに追加
target_include_directories(Core_Lib PUBLIC "${Core_Config_ROOT}/Include_Public")
