# ==================================================
# Physics(当たり判定などの物理処理) の CMakeLists.txt
# ==================================================

add_subdirectory(0_Collider)
add_subdirectory(1_Collision)

# 関数定義
function(Add_Physics TARGET)
	Add_Collider(${TARGET})
	Add_Collision(${TARGET})
endfunction()
