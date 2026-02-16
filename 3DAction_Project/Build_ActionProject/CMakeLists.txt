# --------------------------------------
# CMake 設定
# --------------------------------------
# CMake の最小バージョン指定
cmake_minimum_required(VERSION 3.21)
# プロジェクト名と使用言語指定
project(3DAction_Project LANGUAGES CXX)

# --------------------------------------
# C++ 設定
# --------------------------------------
# C++20 を使用
set(CMAKE_CXX_STANDARD 20)
# C++20 を必須に設定
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# 独自拡張機能を無効化
set(CMAKE_CXX_EXTENSIONS OFF)


# --------------------------------------
# MSVC(VisualStudio C++ コンパイラ) 時の設定
# MT / MTd ランタイムライブラリを使用する
# --------------------------------------
if(MSVC)
    # MT / MTd に設定
    # $<$<条件>:値>
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()


# --------------------------------------
# 開発フォルダーへのパス設定
# --------------------------------------
# 自分の場所からの相対パスで指定
set(DEVELOPMENT_DIR "${CMAKE_CURRENT_LIST_DIR}/../3DAction_Project")
# CPP のあるフォルダーをソースフォルダーとして設定
set(SRC_DIR "${DEVELOPMENT_DIR}")
# .h  のあるフォルダーをインクルードフォルダーとして設定
set(INCLUDE_DIR "${DEVELOPMENT_DIR}")


# --------------------------------------
# Windows 用ライブラリのパス
# --------------------------------------
if(WIN32)
# インクルードパス設定 グローバルに設定
    include_directories(
        "C:/0_DXLibs/0_Assimp/5.2.5/include"
        "C:/0_DXLibs/0_DirectXTex_2019/include"
    )

    # デバッグ時ライブラリパス設定
    set(ASSIMP_LIB_DEBUG "C:/0_DXLibs/0_Assimp/5.2.5/lib/assimp-vc143-mtd.lib")
    set(DXTEXTURE_LIB_DEBUG "C:/0_DXLibs/0_DirectXTex_2019/pdb/Debug/DirectXTex.lib")
    
    # リリース版ライブラリパス設定
    set(ASSIMP_LIB_RELEASE "C:/0_DXLibs/0_Assimp/5.2.5/lib/assimp-vc143-mtd.lib")
    set(DXTEXTURE_LIB_RELEASE "C:/0_DXLibs/0_DirectXTex_2019/pdb/Relese/DirectXTex.lib")
endif()


# --------------------------------------
# ソースファイル
# --------------------------------------
file(GLOB_RECURSE SRC_FILES "${SRC_DIR}/*.cpp")
message(STATUS "SRC_FILES = ${SRC_FILES}")

# Windows専用のDirectXソースを追加
if (WIN32)
    file(GLOB_RECURSE DX_SRC "${CMAKE_SOURCE_DIR}/1_Platform/Widndows/0_WindowsLibrary/*.cpp" "${CMAKE_SOURCE_DIR}/1_Platform/Widndows/0_WindowsLibrary/*.h")
    file(GLOB_RECURSE DX_SRC "${CMAKE_SOURCE_DIR}/1_Platform/Widndows/1_DirectX11/*.cpp" "${CMAKE_SOURCE_DIR}/1_Platform/Widndows/1_DirectX11/*.h")
    list(APPEND SRC_FILES ${DX_SRC})
    message(STATUS "SRC_FILES (with DirectX) = ${SRC_FILES}")
endif()


# --------------------------------------
# 実行ファイル / ライブラリ生成
# --------------------------------------
if (WIN32)
    add_compile_definitions(UNICODE _UNICODE)
endif()

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

# --------------------------------------
# ターゲット設定（共通）
# --------------------------------------
target_include_directories(${PROJECT_NAME}
    PRIVATE ${INCLUDE_DIR}
)

# Unicode（Windowsのみ）
if(WIN32)
    target_compile_definitions(${PROJECT_NAME}
        PRIVATE UNICODE _UNICODE
    )
endif()


# --------------------------------------
# Windows 専用リンク
# --------------------------------------
if(WIN32)
    target_link_libraries(${PROJECT_NAME} 
        debug ${ASSIMP_LIB_DEBUG} 
        optimized ${ASSIMP_LIB_RELEASE}
        debug ${DXTEXTURE_LIB_DEBUG}
        optimized ${DXTEXTURE_LIB_RELEASE}
    )
endif()


# --------------------------------------
# 警告レベル設定
# --------------------------------------
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /permissive-)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()
