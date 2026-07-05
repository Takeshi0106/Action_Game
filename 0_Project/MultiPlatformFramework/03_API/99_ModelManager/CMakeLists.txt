# =============================================
# Core Model 用の CMakeLists.txt
# =============================================

# 階層を取得
set(Core_Model_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# =====================================
# Assimpのダウンロード
# =====================================
# Windows
if(PLATFORM_OS STREQUAL "WINDOWS")
    if(NOT EXISTS "${Core_Model_ROOT}/Assimp/5.2.5/Windows")
        # AssimpのURLを設定
        set(ASSIMP_WINDOWS_URL "https://github.com/Takeshi0106/Assimp-Prebuilt/releases/download/5.2.5/5.2.5.zip")
        # Assimpを入れるフォルダのパス
        set(ASSIMP_WINDOWS_ROOT "${Core_Model_ROOT}/Assimp")
        file(MAKE_DIRECTORY "${ASSIMP_WINDOWS_ROOT}")
        # 一時的に保存するパス
        set(TMP_ZIP "${CMAKE_BINARY_DIR}/assimp.zip")
        set(TMP_DIR "${CMAKE_BINARY_DIR}/assimp_tmp")

        message(STATUS "Assimp not found. Downloading...")
    
        # ダウンロード処理
        file(DOWNLOAD ${ASSIMP_WINDOWS_URL} ${TMP_ZIP} SHOW_PROGRESS)
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
                ${ASSIMP_WINDOWS_ROOT}/${name})
        endforeach()
    
        # 展開に使用した一時ファイルを削除
        file(REMOVE_RECURSE ${TMP_DIR})
        file(REMOVE ${TMP_ZIP})
    else()
        message(STATUS "Assimp already exists. Skip download.")
    endif()

    # ==============================================
    # Assimpを設定
    # ==============================================
    # インクルードディレクトリーに追加
    target_include_directories(API_Lib PRIVATE "${Core_Model_ROOT}/Assimp/5.2.5/include")

    # lib設定
    target_link_libraries(API_Lib PRIVATE
        debug     "${Core_Model_ROOT}/Assimp/5.2.5/Windows/lib/assimp-vc145-mtd.lib"
        optimized "${Core_Model_ROOT}/Assimp/5.2.5/Windows/lib/assimp-vc145-mt.lib")
    
    # DLLのパスを親に渡す
    set(ASSIMP_DEBUG_DLL
        "${Core_Model_ROOT}/Assimp/5.2.5/Windows/dll/assimp-vc145-mtd.dll" PARENT_SCOPE)
    set(ASSIMP_RELEASE_DLL
        "${Core_Model_ROOT}/Assimp/5.2.5/Windows/dll/assimp-vc145-mt.dll" PARENT_SCOPE)

else()
    message(WARNING "Assimp is not set up for this platform. Please set it up manually.")

endif()

# ==============================================
# 自作Modelソースコード追加
# ==============================================
# インクルードディレクトリーに追加
target_include_directories(API_Lib PUBLIC "${Core_Model_ROOT}/Include_Private")

# ソースファイルを追加
target_sources(API_Lib PRIVATE
	"${Core_Model_ROOT}/Src/ModelLoadeModule.cpp"
	"${Core_Model_ROOT}/Src/ModelManager.cpp"
	"${Core_Model_ROOT}/Src/ModelConversionModule.cpp")
