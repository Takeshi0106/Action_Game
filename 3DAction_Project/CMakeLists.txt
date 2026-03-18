# --------------------------------------
# CMake 設定
# --------------------------------------
# CMake の最小バージョン指定
cmake_minimum_required(VERSION 3.21)
# プロジェクト名と使用言語指定
project(3DAction_Project LANGUAGES CXX C)

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
# MSVC・Clang-CL 時の設定
# 静的ランタイム
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
# すべてのOS,APIで使用するソースファイル設定
# SRC_DIR以下にあるすべての.cppを自動収集して変数に入れています。
file(GLOB SRC_FILES "${SRC_DIR}/*.cpp")


# --------------------------------------
# ターゲット指定
# --------------------------------------
# SRC_FILES が追加した.cppをビルド対象に .exe を作成する命令
add_executable(${PROJECT_NAME} ${SRC_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE
    "${SRC_DIR}")


# --------------------------------------
# Assimp 設定
# --------------------------------------
# パスを設定
set(ASSIMP_INCLUDE_DIR "C:/0_DXLibs/0_Assimp/5.2.5/include")

# Windows OS 上でビルドを行っているかのif文です。 (Win32,Win64 ではありません)
if(WIN32)
    # Debug時の AssimpLib パス
    set(ASSIMP_LIB_DEBUG   "C:/0_Assimp/5.2.5/lib/assimp-vc143-mtd.lib")
    # Release時の AssimpLib パス
    set(ASSIMP_LIB_RELEASE "C:/0_Assimp/5.2.5/lib/assimp-vc143-mtd.lib")
else()
    # OSが自動で探す (見つからなかったら、停止させる)
    find_package(assimp REQUIRED)

    # 自動で見つけたパスを設定
    set(ASSIMP_LIB_DEBUG assimp)
    set(ASSIMP_LIB_RELEASE assimp)
endif()

# プロジェクトにヘッダーファイルを追加
# PRIVATE は PROJECT_NAME のターゲットのみに追加する
target_include_directories(${PROJECT_NAME} PRIVATE ${ASSIMP_INCLUDE_DIR})

# プロジェクトにライブラリを追加
target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
     $<$<CONFIG:Debug>:${ASSIMP_LIB_DEBUG}>
     $<$<CONFIG:Release>:${ASSIMP_LIB_RELEASE}>)


# --------------------------------------
# Imgui 設定
# --------------------------------------
# Imgui のソースファイルをビルド対象に追加
set(IMGUI_SOURCES
    "${SRC_DIR}/4_Imgui/imgui.cpp"
    "${SRC_DIR}/4_Imgui/imgui_demo.cpp"
    "${SRC_DIR}/4_Imgui/imgui_draw.cpp"
    "${SRC_DIR}/4_Imgui/imgui_tables.cpp"
    "${SRC_DIR}/4_Imgui/imgui_widgets.cpp")

# Imgui のソースファイルをビルド対象に追加
target_sources(${PROJECT_NAME} PRIVATE $<$<CONFIG:Debug>:${IMGUI_SOURCES}>)

# Imgui のインクルードパスを追加 IDEでヘッダーファイルを見つけられるようにするため
target_include_directories(${PROJECT_NAME} PRIVATE
    "${SRC_DIR}/4_Imgui")


# Windows でビルドする場合、Imgui のインクルードパスを追加
if(WIN32)
    # Win32 DX11 で追加
    set(IMGUI_Win32DX11
        "${SRC_DIR}/4_Imgui/imgui_impl_win32.cpp"
        "${SRC_DIR}/4_Imgui/imgui_impl_dx11.cpp")

    # Imgui のソースファイルをビルド対象に追加
    target_sources(${PROJECT_NAME} PRIVATE $<$<CONFIG:Debug>:${IMGUI_Win32DX11}>)

    # Imgui のインクルードパスを追加
    target_include_directories(${PROJECT_NAME} PRIVATE
    "${SRC_DIR}/4_Imgui")
endif()


# --------------------------------------
# SIPRV-Reflect 設定
# --------------------------------------
# SIRV-V のパスを設定
set(SPIRV_REF_SRC "${SRC_DIR}/3_SPIRV/spirv_reflect.c")
# SPIR-V Reflect のソースファイルをビルド対象に追加
add_library(SPIRV_REF_C STATIC "${SPIRV_REF_SRC}")  

# SPIR-V Reflect のインクルードパスを追加
# PUBLIC は SPIRV_REF_C をリンクするターゲットにもインクルードパスを追加する
target_include_directories(SPIRV_REF_C PUBLIC
    "C:/0_Spirv/include")

# Windows でビルドする場合、SPIR-V Reflect をビルドする際のプリプロセッサ定義を追加
if(WIN32)
# SPIR-V Reflect をビルドする際のプリプロセッサ定義を追加
target_compile_definitions(SPIRV_REF_C PRIVATE SPIRV_REFLECT_USE_SYSTEM_SPIRV_H)
endif()

# SPIR-V Reflect ライブラリをプロジェクトにリンク
target_link_libraries(${PROJECT_NAME} PRIVATE SPIRV_REF_C)

# --------------------------------
# リファレンス実装の追加
# --------------------------------
# SPIR-V Reflect のソースファイルをビルド対象に追加
set(SPIRV_REF "${SRC_DIR}/3_SPIRV/SPIRVReferenctModule.cpp")
# 既存ターゲットにソースを追加
target_sources(${PROJECT_NAME} PRIVATE ${SPIRV_REF})


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
# プリプロセッサ定義の追加
# --------------------------------------
if (WIN32)
    # Windows で Unicode を使用するための定義を追加
    target_compile_definitions(
        ${PROJECT_NAME} PRIVATE 
    UNICODE 
    _UNICODE)

elseif(ANDROID)
    add_library(${PROJECT_NAME} SHARED ${SRC_FILES} ${SPIRV_REF_SRC})

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
        PRIVATE UNICODE _UNICODE SPIRV_REFLECT_USE_SYSTEM_SPIRV_H
    )
endif()


# --------------------------------------
# 警告レベル設定
# --------------------------------------
# MSVC なら /W4 と /permissive- を追加    
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /permissive-)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

# --------------------------------------
# C++20 をターゲットに設定
# --------------------------------------
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
