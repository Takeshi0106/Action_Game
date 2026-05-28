# ===============================================
# PCOS 用の CMakeLists.txt
# ===============================================

# サブディレクトリーを追加
add_subdirectory(0_File)

# PCOS 用の関数を定義
function(ADD_PCOS TARGET)
	Add_PCOS_FileUtils(${TARGET})
endfunction()
