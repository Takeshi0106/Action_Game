# ==================================================
# File の CMakeLists.txt
# ==================================================

# 関数定義
function(Add_FileUtils TARGET)
	# 現在の階層を取得
	set(FileUTils_ROOT ${CMAKE_CURRENT_FUNCTION_LIST_DIR})

	# インクルードディレクトリーに追加
	target_include_directories(${TARGET} PRIVATE "${FileUTils_ROOT}/Include")

	# ソースファイルを追加
	target_sources(${TARGET} PRIVATE
		"${FileUTils_ROOT}/Src/SaveLoadUtiles.cpp")
endfunction()
