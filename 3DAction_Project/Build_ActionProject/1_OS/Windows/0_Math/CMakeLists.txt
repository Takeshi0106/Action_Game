# ==========================================================
# Windows 用の Math の CMakeLists.txt
# ==========================================================

# 関数
function(Add_WIN_Math TARGET)

    # 階層を取得
    set(WIN_MATH_ROOT ${CMAKE_CURRENT_FUNCTION_LIST_DIR})
    
    # インクルードディレクトリーに追加
    target_include_directories(${TARGET} PRIVATE "${WIN_MATH_ROOT}/Include")
    
    # ソースファイルを追加
    target_sources(${TARGET} PRIVATE
	    "${WIN_MATH_ROOT}/Src/Windows_Color.cpp"
        "${WIN_MATH_ROOT}/Src/Windows_Vector2.cpp"
        "${WIN_MATH_ROOT}/Src/Windows_Vector3.cpp"
        "${WIN_MATH_ROOT}/Src/Windows_Vector4.cpp"
        "${WIN_MATH_ROOT}/Src/Windows_Quaternion.cpp"
        "${WIN_MATH_ROOT}/Src/Windows_Matrix3x3.cpp"
        "${WIN_MATH_ROOT}/Src/Windows_Matrix4x4.cpp"
        "${WIN_MATH_ROOT}/Src/WindowsMath_Utiles.cpp")
endfunction()
