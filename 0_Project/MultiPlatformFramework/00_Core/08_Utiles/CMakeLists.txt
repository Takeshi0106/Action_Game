# =========================================================
# Core Utiles の CMakeLists.txt
# =========================================================

# 階層を取得
set(Core_Utiles_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# インクルードディレクトリーに追加
target_include_directories(Core_Lib PUBLIC "${Core_Utiles_ROOT}/Include_Public")

# ソースファイルを追加
target_sources(Core_Lib PRIVATE
	"${Core_Utiles_ROOT}/Src/ShaderUtility.cpp")
