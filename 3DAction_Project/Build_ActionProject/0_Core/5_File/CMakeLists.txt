# ==================================================
# File の CMakeLists.txt
# ==================================================

# 現在の階層を取得
set(FileUTils_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# インクルードディレクトリーに追加
target_include_directories(Core_Lib PUBLIC "${FileUTils_ROOT}/Include")

# ソースファイルを追加
target_sources(Core_Lib PUBLIC
	"${FileUTils_ROOT}/Src/SaveLoadUtiles.cpp")
