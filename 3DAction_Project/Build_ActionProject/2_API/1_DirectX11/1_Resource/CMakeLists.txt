# ==================================================
# DirectX11APIResource の CMakeLists.txt
# ==================================================

# サブディレクトリーを追加
add_subdirectory(0_Object)
add_subdirectory(1_Struct)
add_subdirectory(2_Loader)
add_subdirectory(3_Manager)
add_subdirectory(4_Factory)

# 関数を定義
function(Add_DX11_Resource TARGET)
    Add_DX11APIObject(${TARGET})
	Add_DX11Struct(${TARGET})
	Add_DX11Loader(${TARGET})
	Add_DX11APIManager(${TARGET})
	Add_DX11Factory(${TARGET})
endfunction()
