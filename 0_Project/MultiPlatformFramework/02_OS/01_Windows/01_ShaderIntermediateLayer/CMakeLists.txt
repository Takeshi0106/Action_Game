# ==============================================
# Spirv Compiler (DXC) の CMakeLists.txt
# ==============================================

# DXCのインクルード、ライブラリーを設定
target_include_directories(OS_Lib PRIVATE "${DXC_ROOT}/inc")
target_link_libraries(OS_Lib PRIVATE "${DXC_ROOT}/lib/dxcompiler.lib" "${DXC_ROOT}/lib/dxil.lib")

# インクルードディレクトリーに追加
target_include_directories(OS_Lib PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/Include_Public")
# Srcを追加
target_sources(OS_Lib PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/Src/Windows_SPIRVCompileModule.cpp")

# カスタムイベントでdllのパスを使うため設定
set(DXC_DLL "${DXC_ROOT}/bin/dxcompiler.dll" CACHE INTERNAL "DXCompiler DLL")
set(DXIL_DLL "${DXC_ROOT}/bin/dxil.dll" CACHE INTERNAL "DXIL DLL")
