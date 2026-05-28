# ================================================
# Windows 用の CMakeLists.txt
# ================================================

# サブディレクトリーを追加
add_subdirectory(0_Math)
add_subdirectory(1_Timer)
add_subdirectory(2_Report)

# Windows 用の関数を定義
function(ADD_WINDOWS TARGET)
    ADD_WIN_MATH(${TARGET})
	Add_WIN_Timer(${TARGET})
	Add_WIN_Report(${TARGET})
endfunction()
