# =========================================
# SPIR-V Reflect の CMakeLists.txt
# =========================================
# 現在のCmakeListの階層を取得する
set(SPIRV_REFLECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# =========================================
# 静的ライブラリを作成
# =========================================
add_library(SPIRV_REF_LIB STATIC
    "${SPIRV_REFLECT_ROOT}/Src/spirv_reflect.c"
    "${SPIRV_REFLECT_ROOT}/Src/SPIRVReferenceModule.cpp"
    "${SPIRV_REFLECT_ROOT}/Src/SPIRV_SelfRefDataModule.cpp")

# =========================================
# SPIRV 共有ライブラリを取得
# =========================================
target_link_libraries(SPIRV_REF_LIB PRIVATE SPIRV_COMMON)

# =========================================
# インクルードディレクトリーに追加
# =========================================
# 外部に公開しない
target_include_directories(SPIRV_REF_LIB PRIVATE 
    "${SPIRV_REFLECT_ROOT}/Src")
# 外部に公開する
target_include_directories(SPIRV_REF_LIB PUBLIC
    "${SPIRV_REFLECT_ROOT}/Include")
