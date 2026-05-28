# =========================================
# SPIR-V Reflect の CMakeLists.txt
# =========================================
# 現在のCmakeListの階層を取得する
set(SPIRV_REFLECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# =========================================
# 静的ライブラリを作成
# =========================================
# C ライブラリ
add_library(SPIRV_REFLECT_C STATIC
    "${SPIRV_REFLECT_ROOT}/Src/spirv_reflect.c")

add_library(SPIRV_REF_LIB STATIC
    "${SPIRV_REFLECT_ROOT}/Src/SPIRVReferenceModule.cpp"
    "${SPIRV_REFLECT_ROOT}/Src/SPIRVReflectionUtils.cpp")

# =========================================
# SPIRV 共有ライブラリを取得
# =========================================
ADD_SPIRV_COMMON(SPIRV_REFLECT_C)
ADD_SPIRV_COMMON(SPIRV_REF_LIB)

# =========================================
# C をリンク
# =========================================
target_link_libraries(SPIRV_REF_LIB PRIVATE
    SPIRV_REFLECT_C)

# =========================================
# IDE 解析をC++20に
# =========================================
target_compile_features(SPIRV_REF_LIB PUBLIC cxx_std_20)
target_compile_features(SPIRV_REFLECT_C PUBLIC c_std_11)

# =========================================
# インクルードディレクトリーに追加
# =========================================
# 外部に公開しない
target_include_directories(SPIRV_REF_LIB PRIVATE 
    "${SPIRV_REFLECT_ROOT}/Src")
# 外部に公開する
target_include_directories(SPIRV_REF_LIB PUBLIC
    "${SPIRV_REFLECT_ROOT}/Include")
