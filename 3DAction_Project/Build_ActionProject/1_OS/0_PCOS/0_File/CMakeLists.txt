# ==========================================================
# PCOS 用の FileUtils の CMakeLists.txt
# ==========================================================

# 関数
function(Add_PCOS_FileUtils TARGET)

    # 階層を取得
    set(PCOS_FILEUTILS_ROOT ${CMAKE_CURRENT_FUNCTION_LIST_DIR})
    
    # ソースファイルを追加
    target_sources(${TARGET} PRIVATE
	    "${PCOS_FILEUTILS_ROOT}/Src/FileUtils.cpp")
endfunction()
