# ============================================
# String の CMakeLists.txt
# ============================================

# 関数定義
function(Add_String TARGET)
	
	# 現在の階層を取得
	set(STRING_ROOT ${CMAKE_CURRENT_FUNCTION_LIST_DIR})

	# インクルードディレクトリーに追加
	target_include_directories(${TARGET} PRIVATE "${STRING_ROOT}/Include")

	# ソースファイルを追加
    target_sources(${TARGET} PRIVATE
	    "${STRING_ROOT}/Src/UTF8_String.cpp")

endfunction()
