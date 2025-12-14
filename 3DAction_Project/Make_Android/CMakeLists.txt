cmake_minimum_required(VERSION 3.21)
project(3DAction_Project LANGUAGES CXX)

# --------------------------------------
# C++ 設定
# --------------------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --------------------------------------
# MSVC ランタイム統一
# --------------------------------------
if(MSVC)
    # Debug は静的デバッグランタイム /MTd
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

# --------------------------------------
# 開発フォルダーへの相対パス
# --------------------------------------
set(DEVELOPMENT_DIR "${CMAKE_CURRENT_LIST_DIR}/../3DAction_Project")
set(SRC_DIR "${DEVELOPMENT_DIR}")
set(INCLUDE_DIR "${DEVELOPMENT_DIR}")

include_directories(${INCLUDE_DIR})

# --------------------------------------
# Windows 用ライブラリのパス
# --------------------------------------
if(WIN32)
    include_directories(
        "C:/0_DXLibs/0_Assimp/5.2.5/include"
        "C:/0_DXLibs/0_DirectXTex_2019/include"
    )

    # ライブラリパス設定
    set(ASSIMP_LIB_DEBUG "C:/0_DXLibs/0_Assimp/5.2.5/lib/assimp-vc143-mtd.lib")
    set(ASSIMP_LIB_RELEASE "C:/0_DXLibs/0_Assimp/5.2.5/lib/assimp-vc143-mtd.lib")

    set(DXTEXTURE_LIB_DEBUG "C:/0_DXLibs/0_DirectXTex_2019/pdb/Debug/DirectXTex.lib")
    set(DXTEXTURE_LIB_RELEASE "C:/0_DXLibs/0_DirectXTex_2019/pdb/Relese/DirectXTex.lib")
endif()

# --------------------------------------
# ソースファイル
# --------------------------------------
file(GLOB_RECURSE SRC_FILES "${SRC_DIR}/*.cpp")
message(STATUS "SRC_FILES = ${SRC_FILES}")

# --------------------------------------
# 実行ファイル / ライブラリ生成
# --------------------------------------
if(ANDROID)
    add_library(${PROJECT_NAME} SHARED ${SRC_FILES})
else()
    add_executable(${PROJECT_NAME} ${SRC_FILES})

    target_link_libraries(${PROJECT_NAME} 
        debug ${ASSIMP_LIB_DEBUG} 
        optimized ${ASSIMP_LIB_RELEASE}
        debug ${DXTEXTURE_LIB_DEBUG}
        optimized ${DXTEXTURE_LIB_RELEASE}
    )

    target_compile_definitions(${PROJECT_NAME} PRIVATE UNICODE _UNICODE)
endif()
