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
# プラットフォームの設定 (WINDOWS LINUX MAC)
set(PLATFORM_OS "WINDOWS" CACHE STRING "Platform" FORCE)
    if(NOT PLATFORM_OS STREQUAL "WINDOWS" AND
       NOT PLATFORM_OS STREQUAL "LINUX" AND
       NOT PLATFORM_OS STREQUAL "MAC")
        message(FATAL_ERROR "Invalid PLATFORM_OS")
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
# プロジェクトの場所を設定
set(SRC_DIR "${PROJECT_DIR}")

# --------------------------------------
# 実行ファイルを作成
# --------------------------------------
add_executable(${PROJECT_NAME})

# ---------------------------------------
# 各モジュールを追加
# ---------------------------------------
# Core の CMakeLists.txt を追加
add_subdirectory("${SRC_DIR}/0_Core" "${CMAKE_BINARY_DIR}/Core")
target_link_libraries(${PROJECT_NAME} PRIVATE Core_Lib)

# Imgui の CMakeLists.txt を追加
add_subdirectory("${SRC_DIR}/4_Imgui" "${CMAKE_BINARY_DIR}/Imgui")
# Imgui ライブラリをプロジェクトにリンク Debugビルドのときのみリンクする
target_link_libraries(${PROJECT_NAME} PRIVATE $<$<CONFIG:Debug>:IMGUI>)

# OS 用の CMakeLists.txt を追加
add_subdirectory("${SRC_DIR}/1_OS" "${CMAKE_BINARY_DIR}/OS")
target_link_libraries(${PROJECT_NAME} PRIVATE OS_Lib)

# API の CMakeLists.txt を追加
add_subdirectory("${SRC_DIR}/2_API" "${CMAKE_BINARY_DIR}/API")
target_link_libraries(${PROJECT_NAME} PRIVATE API_Lib)

# 絶対パスに変換
get_filename_component(DXC_EXE "${DXC_EXE}" ABSOLUTE)
# #define でコンパイラーのパスをプロジェクトに渡す
target_compile_definitions(${PROJECT_NAME} PRIVATE DXC_PATH=u8\"${DXC_EXE}\")

# Physics 追加
add_subdirectory("${SRC_DIR}/5_Physics" "${CMAKE_BINARY_DIR}/Physices")
# ライブラリを追加
target_link_libraries(${PROJECT_NAME} PRIVATE Physics_Lib)

# SRC_DIR階層にあるすべての.cppを自動収集して変数に入れています。
file(GLOB SRC_FILES "${SRC_DIR}/*.cpp")

target_sources(${PROJECT_NAME} PRIVATE ${SRC_FILES})
# プロジェクトのインクルードパスを追加
target_include_directories(${PROJECT_NAME} PRIVATE "${SRC_DIR}")

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
# Assets フォルダもコピー
add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${SRC_DIR}/Asset"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/Asset)

# Assimp DLL をコピー,
add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
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
