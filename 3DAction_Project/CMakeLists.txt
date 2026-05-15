# --------------------------------------
# CMake 設定
# --------------------------------------
# CMake の最小バージョン指定
cmake_minimum_required(VERSION 3.21)
# プロジェクト名と使用言語指定
project(MultiPlatformFramework LANGUAGES CXX C)

# --------------------------------------
# C++ 設定
# --------------------------------------
# C++20 を使用
set(CMAKE_CXX_STANDARD 20)
# C++20 を必須に設定
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# 独自拡張機能を無効化
set(CMAKE_CXX_EXTENSIONS OFF)
# コンパイル情報を出力する
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# --------------------------------------
# ビルド 設定
# ---------------------------------------
# プラットフォームの設定
set(PLATFORM_OS "WINDOWS" CACHE STRING "Platform")
# API の設定
set(PLATFORM_API "DIRECTX11" CACHE STRING "API")
# SPIRV ライブラリの設定
option(SPIRV_MODULE "Spirv Module" ON)

# --------------------------------------
# 静的リンクでビルド
# --------------------------------------
if(MSVC)
    # $<$<条件>:値>
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

# --------------------------------------
# 開発フォルダーへのパス設定
# --------------------------------------
# このテキストの階層からの相対パスを指定
set(PROJECT_DIR "${CMAKE_CURRENT_LIST_DIR}/Build_ActionProject")
# プロジェクトの場所を設定
set(SRC_DIR "${PROJECT_DIR}")

# --------------------------------------
# ターゲット指定
# --------------------------------------
# SRC_DIR階層にあるすべての.cppを自動収集して変数に入れています。
# IDE がうまく働かないので、後に手動で追加に修正します
file(GLOB SRC_FILES "${SRC_DIR}/*.cpp")

# SRC_FILES で追加した実行ファイルを作成
add_executable(${PROJECT_NAME} ${SRC_FILES})
# プロジェクトのインクルードパスを追加
target_include_directories(${PROJECT_NAME} PRIVATE "${SRC_DIR}")

# --------------------------------------
# 外部ライブラリの設定
# --------------------------------------
# Assimp の CMakeLists.txt を追加
add_subdirectory("C:/0_Assimp/5.2.5" "${CMAKE_BINARY_DIR}/Assimp")
# Assimp ライブラリをプロジェクトにリンク
target_link_libraries(${PROJECT_NAME} PRIVATE ASSIMP_LIB)

# Imgui の CMakeLists.txt を追加
add_subdirectory("${SRC_DIR}/5_Imgui" "${CMAKE_BINARY_DIR}/Imgui")
# Imgui ライブラリをプロジェクトにリンク Debugビルドのときのみリンクする
target_link_libraries(${PROJECT_NAME} PRIVATE $<$<CONFIG:Debug>:IMGUI>)

# SPIR-V Reflect の CMakeLists.txt を追加
add_subdirectory("${SRC_DIR}/3_SPIRV" "${CMAKE_BINARY_DIR}/SPIRV")
# SPIR-V Reflect ライブラリをプロジェクトにリンク
target_link_libraries(${PROJECT_NAME} PRIVATE SPIRV_REF_LIB)

# --------------------------------------
# Windows 用ライブラリのパス
# --------------------------------------
if(WIN32)
    # --------------------------------------
    # DirectX Tex 追加
    # --------------------------------------
    # インクルードパス設定
    target_include_directories(${PROJECT_NAME} PRIVATE
        "C:/0_DXLibs/0_DirectXTex_2019/include")

    # ライブラリパスを設定
    target_link_libraries(
        ${PROJECT_NAME}
        PRIVATE
          $<$<CONFIG:Debug>:"C:/0_DXLibs/0_DirectXTex_2019/lib/MT/Debug/DirectXTex">
          $<$<CONFIG:Release>:"C:/0_DXLibs/0_DirectXTex_2019/lib/MT/Release/DirectXTex">)
endif()


# --------------------------------------
# プラットフォーム・API別のソースファイル追加
# --------------------------------------
# Windows専用のDirectXソースを追加
if (WIN32)
    # Windows ライブラリソースを追加
    file(GLOB WIN_MATH_SRC "${SRC_DIR}/1_OS/Windows/0_Math/*.cpp")
    file(GLOB WIN_TIMER_SRC "${SRC_DIR}/1_OS/Windows/1_Timer/*.cpp")
    file(GLOB WIN_REPORT_SRC "${SRC_DIR}/1_OS/Windows/2_Report/*.cpp")
    file(GLOB WIN_SPIRV_SRC "${SRC_DIR}/1_OS/Windows/3_SPIRV/*.cpp")

    # Windowsソースを追加
    target_sources(${PROJECT_NAME} PRIVATE 
    ${WIN_MATH_SRC} 
    ${WIN_TIMER_SRC}
    ${WIN_REPORT_SRC}
    ${WIN_SPIRV_SRC})

    # Windows DirectX11 APIソースを追加
    file(GLOB DX_API_SRC "${SRC_DIR}/1_Platform/Windows/1_DirectX11/*.cpp")

    # 既存ターゲットにソースを追加
    target_sources(${PROJECT_NAME} PRIVATE ${DX_API_SRC})

 # 開発時(Debug時のみに使用する)ソース追加
    file(GLOB DX_DEV_SRC "${SRC_DIR}/1_Platform/Windows/1_DirectX11/0_Development/*.cpp")

    # Debugビルドのときのみ、開発用ソースを追加
    target_sources(${PROJECT_NAME} PRIVATE $<$<CONFIG:Debug>:${DX_DEV_SRC}>)

    # ソースファイルが含まれているかチェック用
    # message(STATUS "WIN_TIMER_SRC = ${WIN_TIMER_SRC}")
    # message(STATUS "WIN_REPORT_SRC = ${WIN_REPORT_SRC}")
    # message(STATUS "WIN_SPIRV_SRC = ${WIN_SPIRV_SRC}")

    # OS のほう
    # message(STATUS "DX_LIB_SRC = ${DX_LIB_SRC}")
    message(STATUS "DX_API_SRC = ${DX_API_SRC}")
    message(STATUS "DX_DEV_SRC = ${DX_DEV_SRC}")
endif()


# --------------------------------------
# プリプロセッサ定義
# --------------------------------------
# SPIR-V Reflect をビルドする際のプリプロセッサ定義
target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        SPIRV_REFLECT_USE_SYSTEM_SPIRV_H
)

# Windows で Unicode を使用するための定義を追加
if (WIN32)
    target_compile_definitions(
        ${PROJECT_NAME} PRIVATE 
    UNICODE 
    _UNICODE)

endif()


# --------------------------------------
# ターゲット設定（共通）
# --------------------------------------
# 共通のインクルードパスを追加
target_include_directories(${PROJECT_NAME}
    PRIVATE ${INCLUDE_DIR}
)

# --------------------------------------
# 警告レベル設定
# --------------------------------------
# MSVC なら /W4(警告レベル) と /permissive-(C++標準強化) を追加    
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /permissive-)

# Clang-CL なら -Wall(全警告) -Wextra(追加警告) -Wpedantic(C++標準強化) を追加
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

# --------------------------------------
# C++20 をターゲットに設定
# --------------------------------------
# このプロジェクトで C++20 を使用することを指定
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

# --------------------------------------
# カスタムイベント (ビルド後イベント)
# --------------------------------------
# Assimp DLL をコピー
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "C:/0_Assimp/5.2.5/bin/assimp-vc143-mtd.dll"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
)

# Assets フォルダもコピー
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${SRC_DIR}/0_Asset"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/0_Asset
)
