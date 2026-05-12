# =========================================
# SPIR-V Reflect の CMakeLists.txt
# =========================================
# 静的ライブラリを作成
# 静的ライブラリ作成
add_library(SPIRV_REF_LIB STATIC
    "${SRC_DIR}/3_SPIRV/spirv_reflect.c"
    "${SRC_DIR}/3_SPIRV/SPIRVReferenctModule.cpp")

# ======================================
# インクルードディレクトリーに追加
# ======================================
target_include_directories(SPIRV_REF_LIB PUBLIC
    "C:/0_Spirv/include")

# Windows
if(WIN32)
    # プリプロセッサ定義を追加
    target_compile_definitions(SPIRV_REF_LIB PRIVATE
        SPIRV_REFLECT_USE_SYSTEM_SPIRV_H)

endif()
