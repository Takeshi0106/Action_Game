# ==================================================
# DirectX11API の CMakeLists.txt
# ==================================================

# サブディレクトリーを追加
add_subdirectory(0_Utiles)
add_subdirectory(1_Resource)
add_subdirectory(2_Shader)

# 関数を定義
function(Add_DX11 TARGET)
    Add_DX11Utiles(${TARGET})
	Add_DX11_Resource(${TARGET})
	Add_DX11_Shader(${TARGET})
endfunction()
