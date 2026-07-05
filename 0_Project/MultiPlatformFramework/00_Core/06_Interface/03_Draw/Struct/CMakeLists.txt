# ============================================
# DrawConfig のCMakeLists.txt
# ============================================

set(Interface_Draw_Struct_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# インクルードディレクトリーに追加
target_include_directories(Core_Lib PUBLIC "${Interface_Draw_Struct_ROOT}/Include_Public")
