# =========================================================
# API の CMakeLists.txt
# =========================================================
# ライブラリ作成
add_library(API_Lib STATIC)

# ライブラリーをリンク
target_link_libraries(API_Lib PRIVATE Core_Lib)
target_link_libraries(API_Lib PRIVATE OS_Lib)
target_link_libraries(API_Lib PRIVATE IMGUI)

# DirectX11API
if(PLATFORM_API STREQUAL "DIRECTX11")
    add_subdirectory(01_DirectX11)
endif()

# 共通のModelManager
add_subdirectory(99_ModelManager)

# デファインに API を追加
if(PLATFORM_API STREQUAL "DIRECTX11")
    target_compile_definitions(API_Lib PRIVATE PLATFORM_API_DIRECTX11)
else()
    message(FATAL_ERROR "Unsupported PLATFORM_API")
endif()

# Assimp ライブラリーのパスを親スコープに設定
set(ASSIMP_DEBUG "${ASSIMP_DEBUG_DLL}" PARENT_SCOPE)
set(ASSIMP_RELEASE "${ASSIMP_RELEASE_DLL}" PARENT_SCOPE)
