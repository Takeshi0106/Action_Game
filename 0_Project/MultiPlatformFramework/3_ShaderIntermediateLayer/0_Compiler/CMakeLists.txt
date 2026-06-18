# =====================================
# SPIRV Compiler の CMakeLists.txt
# =====================================
# 現在のCmakeListの階層を取得する
set(SPIRV_COMPILER_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# =====================================
# シェーダーコンパイラーのダウンロード
# =====================================
# Windows
if(PLATFORM_OS STREQUAL "WINDOWS")
    if(NOT EXISTS "${SPIRV_COMPILER_ROOT}/0_Windows/dxc/dxc.exe")
        # DXCのURLを設定
        set(DXC_WINDOWS_URL "https://github.com/Takeshi0106/DXC-Prebuilt/releases/download/v1.0/dxc_2026_0220.zip")
        # DXCを入れるフォルダのパス
        set(DXC_WINDOWS_ROOT "${SPIRV_COMPILER_ROOT}/0_Windows/dxc")
        # 一時的に.zipファイルを保存するパス
        set(TMP_ZIP "${CMAKE_BINARY_DIR}/dxc.zip")
        # 一時的に展開するフォルダのパス
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
            file(RENAME
                ${item}
                ${DXC_WINDOWS_ROOT}/${name})
        endforeach()
    
        # 展開に使用した一時ファイルを削除
        file(REMOVE_RECURSE ${TMP_DIR})
        file(REMOVE ${TMP_ZIP})
    else()
        message(STATUS "DXC already exists. Skip download.")
    endif()
endif()

# =====================================
# シェーダーコンパイラーを設定 
# =====================================
# Windows
if(PLATFORM_OS STREQUAL "WINDOWS")
    add_subdirectory("${SPIRV_COMPILER_ROOT}/0_Windows")
endif()

# ======================================
# シェーダーコンパイラーのパスを定義
# ======================================
# Windows
if(PLATFORM_OS STREQUAL "WINDOWS")
    set(DXC_EXE
        "${SPIRV_COMPILER_ROOT}/0_Windows/dxc/dxc.exe"
        PARENT_SCOPE)
endif()
