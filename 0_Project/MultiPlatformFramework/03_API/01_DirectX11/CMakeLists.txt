# ==================================================
# DirectX11API の CMakeLists.txt
# ==================================================

# 階層を取得
set(DX11_API_LOADER_V1_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
# DXTexを入れるフォルダのパス
set(DXTex_ROOT "${DX11_API_LOADER_V1_ROOT}/DXTex")

# ダウンロード
if(NOT EXISTS "${DX11_API_LOADER_V1_ROOT}/DXTex/lib/MT/Release/DirectXTex.lib")
    # DXTexのURLを設定
    set(DXTex_URL "https://github.com/Takeshi0106/DirectXTex-Prebuilt/releases/download/2019/DirectXTex_2019.zip")
    file(MAKE_DIRECTORY "${DXTex_ROOT}")
    # 一時的に保存するパス
    set(TMP_ZIP "${CMAKE_BINARY_DIR}/dxtex.zip")
    set(TMP_DIR "${CMAKE_BINARY_DIR}/dxtex_tmp")

    message(STATUS "DXTex not found. Downloading...")
    message(STATUS "Downloading from: ${DXTex_ROOT}")
    
    # ダウンロード処理
    file(DOWNLOAD ${DXTex_URL} ${TMP_ZIP} SHOW_PROGRESS)
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
            ${DXTex_ROOT}/${name})
    endforeach()
    
    # 展開に使用した一時ファイルを削除
    file(REMOVE_RECURSE ${TMP_DIR})
    file(REMOVE ${TMP_ZIP})
else()
    message(STATUS "DXTex already exists. Skip download.")
endif()

# インクルードディレクトリーに追加
target_include_directories(API_Lib PRIVATE
    "${DXTex_ROOT}/Include")
    
# ライブラリパスを設定
target_link_libraries(
    API_Lib
    PRIVATE
      $<$<CONFIG:Debug>:"${DXTex_ROOT}/lib/MT/Debug/DirectXTex">
      $<$<CONFIG:Release>:"${DXTex_ROOT}/lib/MT/Release/DirectXTex">)

# サブディレクトリーを追加
add_subdirectory(0_Utiles)
add_subdirectory(1_Version)
