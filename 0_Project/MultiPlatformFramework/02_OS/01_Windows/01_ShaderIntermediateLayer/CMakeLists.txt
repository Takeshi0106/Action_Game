# ==============================================
# Spirv Compiler (DXC) の CMakeLists.txt
# ==============================================

# インクルードディレクトリーに追加
target_include_directories(OS_Lib PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/Include_Public")

# Srcを追加
target_sources(OS_Lib PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/Src/Windows_SPIRVCompileModule.cpp")
