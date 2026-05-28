# ==============================================
# Spirv Compiler (DXC) の CMakeLists.txt
# ==============================================

# ==============================================
# 静的ライブラリを作成
# ==============================================
add_library(SPIRV_COMPILER_LIB STATIC
	"${CMAKE_CURRENT_SOURCE_DIR}/Src/Windows_SPIRVCompileModule.cpp")

# ==============================================
# SPIRV 共有ライブラリを取得
# ==============================================
ADD_SPIRV_COMMON(SPIRV_COMPILER_LIB)

# ==============================================
# インクルードディレクトリーに追加
# ==============================================
target_include_directories(SPIRV_COMPILER_LIB PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/Include")
