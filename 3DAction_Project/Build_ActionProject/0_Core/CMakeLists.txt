# =============================================
# Core 用の CMakeLists.txt
# =============================================
# サブディレクトリーを追加
add_subdirectory(0_Binary)
add_subdirectory(1_Math)
add_subdirectory(2_String)
add_subdirectory(3_Timer)
add_subdirectory(4_Report)
add_subdirectory(5_File)
add_subdirectory(6_Physics)

# =============================================
# Core を追加
# =============================================
# 関数
function(Add_Core TARGET)
    Add_Binary(${TARGET})
    Add_Math(${TARGET})
    Add_String(${TARGET})
    Add_Timer(${TARGET})
    Add_Report(${TARGET})
    Add_FileUtils(${TARGET})
    Add_Physics(${TARGET})
endfunction()
