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
# プラットフォーム・OS の設定
if(WIN32)
    set(PLATFORM_OS "WINDOWS" CACHE STRING "Platform" FORCE)    
    set(PLATFORM_CATEGORY "PC" CACHE STRING "Platform Category" FORCE)
elseif(APPLE)
    set(PLATFORM_OS "MAC" CACHE STRING "Platform" FORCE)
    set(PLATFORM_CATEGORY "PC" CACHE STRING "Platform Category" FORCE)
elseif(UNIX)
    set(PLATFORM_OS "LINUX" CACHE STRING "Platform" FORCE)
    set(PLATFORM_CATEGORY "PC" CACHE STRING "Platform Category" FORCE)
endif()

# API の設定 (DIRECTX11 VULKAN OPENGL)
set(PLATFORM_API "DIRECTX11" CACHE STRING "API" FORCE)
if(NOT PLATFORM_API STREQUAL "DIRECTX11" AND
   NOT PLATFORM_API STREQUAL "VULKAN" AND
   NOT PLATFORM_API STREQUAL "OPENGL")
    message(FATAL_ERROR "Invalid PLATFORM_API")
endif()

# バージョン設定 (1.0 , 2.0)
set(ARCHITECTURE_VERSION "1.0" CACHE STRING "Architecture version" FORCE)
# シェーダーの中間層を作成するかを設定 (ON OFF)
option(ENABLE_SHADER_INTERMEDIATE_LAYER "Enable shader intermediate layer" ON)

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
set(PROJECT_DIR "${CMAKE_CURRENT_LIST_DIR}/MultiPlatformFramework")

# --------------------------------------
# 実行ファイルを作成
# --------------------------------------
add_executable(${PROJECT_NAME})

# --------------------------------------
# デファイン追加
# --------------------------------------
file(RELATIVE_PATH PROJECT_ASSET_PATH_DEBUG "${CMAKE_BINARY_DIR}""${PROJECT_DIR}/Asset")
set(PROJECT_ASSET_PATH_RELEASE "Asset")
target_compile_definitions(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Debug>:PROJECT_ASSET_PATH=u8\"${PROJECT_ASSET_PATH_DEBUG}\">
    $<$<CONFIG:Release>:PROJECT_ASSET_PATH=${PROJECT_ASSET_PATH_RELEASE}>)

# ---------------------------------------
# 各モジュールを追加
# ---------------------------------------
# 各CMakeLists.txt を実行
add_subdirectory("${PROJECT_DIR}/00_Core" "${CMAKE_BINARY_DIR}/00_Core")
add_subdirectory("${PROJECT_DIR}/01_Imgui" "${CMAKE_BINARY_DIR}/01_Imgui")
add_subdirectory("${PROJECT_DIR}/02_OS" "${CMAKE_BINARY_DIR}/02_OS")
add_subdirectory("${PROJECT_DIR}/03_API" "${CMAKE_BINARY_DIR}/03_API")
add_subdirectory("${PROJECT_DIR}/05_Physics" "${CMAKE_BINARY_DIR}/05_Physics")
add_subdirectory("${PROJECT_DIR}/06_Game" "${CMAKE_BINARY_DIR}/06_Game")
add_subdirectory("${PROJECT_DIR}/07_Main" "${CMAKE_BINARY_DIR}/07_Main")

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
# カスタムイベント
# --------------------------------------
# DXC DLL と DXIL DLL をコピーするカスタムイベントを追加
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/Asset/Dll"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${DXC_DLL}"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/Asset/Dll"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${DXIL_DLL}"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/Asset/Dll")
# HLSLをコピー
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${PROJECT_DIR}/06_Game/05_HLSL"
    "${PROJECT_DIR}/Asset/Shader/Hlsl")
# Assets フォルダもコピー 
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD 
    COMMAND ${CMAKE_COMMAND} -E copy_directory 
    "${PROJECT_DIR}/Asset" $<TARGET_FILE_DIR:${PROJECT_NAME}>/Asset)
# Assimp DLL をコピー,
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo
    "Assimp DLL: $<IF:$<CONFIG:Debug>,${ASSIMP_DEBUG},${ASSIMP_RELEASE}>"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "$<IF:$<CONFIG:Debug>,${ASSIMP_DEBUG},${ASSIMP_RELEASE}>"
    $<TARGET_FILE_DIR:${PROJECT_NAME}>)

# ---------------------------------------
# 設定ログを出力
# ---------------------------------------
# コンパイラーの設定出力
message(STATUS "\n========== Compiler ==========")
message(STATUS "C Compiler   : ${CMAKE_C_COMPILER}")
message(STATUS "CXX Compiler : ${CMAKE_CXX_COMPILER}")
message(STATUS "Compiler ID  : ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "Version      : ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "==============================")

# C++ の設定出力
message(STATUS "\n========== C++ Settings ==========")
message(STATUS "C++ Standard : C++${CMAKE_CXX_STANDARD}")
message(STATUS "Required     : ${CMAKE_CXX_STANDARD_REQUIRED}")
message(STATUS "Extensions   : ${CMAKE_CXX_EXTENSIONS}")
message(STATUS "=================================")

# ビルドの設定出力
message(STATUS "\n========== Build Settings ==========")
message(STATUS "Platform OS  : ${PLATFORM_OS}")
message(STATUS "Platform API : ${PLATFORM_API}")
message(STATUS "Shader Intermediate Layer : ${ENABLE_SHADER_INTERMEDIATE_LAYER}")
message(STATUS "==================================\n")
