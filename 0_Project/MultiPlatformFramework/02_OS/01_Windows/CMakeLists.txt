# ===========================================
# WindowsOS 用の CMakeLists.txt
# ===========================================
# デファインを追加
target_compile_definitions(
    OS_Lib PUBLIC 
UNICODE 
_UNICODE)

add_subdirectory(00_Common)

# バージョン選択
if(ARCHITECTURE_VERSION STREQUAL "1.0")
add_subdirectory(V1.0)

elseif(ARCHITECTURE_VERSION STREQUAL "2.0")
message(FATAL_ERROR "OS 2.0 is not yet implemented")

endif()

# =====================================
# SPIRV Compiler の CMakeLists.txt
# =====================================
# 現在のCmakeListの階層を取得する
set(WINDOWS_OS_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# =====================================
# シェーダーコンパイラーのダウンロード
# =====================================
if(NOT EXISTS "${WINDOWS_OS_ROOT}/01_ShaderIntermediateLayer/dxc/dxc.exe")
    # DXCのURLを設定
    set(DXC_WINDOWS_URL "https://github.com/Takeshi0106/DXC-Prebuilt/releases/download/v1.0/dxc_2026_0220.zip")
    # DXCを入れるフォルダのパス
    set(DXC_WINDOWS_ROOT "${WINDOWS_OS_ROOT}/01_ShaderIntermediateLayer/dxc")
    file(MAKE_DIRECTORY "${DXC_WINDOWS_ROOT}")
    # 一時的に保存するパス
    set(TMP_ZIP "${CMAKE_BINARY_DIR}/dxc.zip")
    set(TMP_DIR "${CMAKE_BINARY_DIR}/dxc_tmp")

    message(STATUS "DXC not found. Downloading...")

    # ダウンロード処理
    file(DOWNLOAD ${DXC_WINDOWS_URL} ${TMP_ZIP} SHOW_PROGRESS)
    # .Zipファイルを展開
    file(ARCHIVE_EXTRACT
        INPUT ${TMP_ZIP}
        DESTINATION ${TMP_DIR})
    
    # 展開したファイルをすべて選択
    file(GLOB CONTENTS "${TMP_DIR}/*")

    # 選択したファイルを１つずつ取り出す
    foreach(item IN LISTS CONTENTS)
        # ファイル名を取得 
        get_filename_component(name ${item} NAME)
        # ファイルを移動
        file(RENAME ${item} ${DXC_WINDOWS_ROOT}/${name})
    endforeach()

    # 展開に使用した一時ファイルを削除
    file(REMOVE_RECURSE ${TMP_DIR})
    file(REMOVE ${TMP_ZIP})
else()
    message(STATUS "DXC already exists. Skip download.")
endif()

# =====================================
# シェーダーコンパイラーを設定 
# =====================================
add_subdirectory("${WINDOWS_OS_ROOT}/01_ShaderIntermediateLayer")

# ======================================
# シェーダーコンパイラーのパスを定義
# ======================================
set(DXC_EXE
    "${WINDOWS_OS_ROOT}/01_ShaderIntermediateLayer/dxc/dxc.exe"
    CACHE INTERNAL
    "DXC executable path")
