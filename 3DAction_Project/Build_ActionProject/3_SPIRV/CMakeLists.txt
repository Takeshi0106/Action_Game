# =======================================
# SPIRV-Tools の CMakeLists.txt
# =======================================
# 現在のCmakeListの階層を取得する
set(SPIRV_TOOLS_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# =======================================
# SPIRV共有の定義
# =======================================
# SPIRVの定義をまとめたライブラリーを作成
add_library(SPIRV_COMMON INTERFACE)
# SPIRV定義ヘッダー
target_include_directories(SPIRV_COMMON INTERFACE
    "${SPIRV_TOOLS_ROOT}")
# マクロ定義
target_compile_definitions(SPIRV_COMMON INTERFACE
    SPIRV_REFLECT_USE_SYSTEM_SPIRV_H)
# ライブラリを追加
target_link_libraries(SPIRV_COMMON INTERFACE Core_Lib)
target_link_libraries(SPIRV_COMMON INTERFACE OS_Lib)

# =======================================
# SPIRV Tool の追加
# =======================================
# SPIRVコンパイラー を追加
add_subdirectory("${SPIRV_TOOLS_ROOT}/0_Compiler")
# SPIRV リファレンス を追加
add_subdirectory("${SPIRV_TOOLS_ROOT}/1_Reference")

# =======================================
# DXCのパスを親スコープに渡す
# =======================================
set(DXC_EXE "${DXC_EXE}" PARENT_SCOPE)
