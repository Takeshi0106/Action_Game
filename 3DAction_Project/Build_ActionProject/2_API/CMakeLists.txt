# =========================================================
# API の CMakeLists.txt
# =========================================================

# DirectX11API
if(PLATFORM_API STREQUAL "DIRECTX11")
    add_subdirectory(1_DirectX11)
endif()

# 関数定義
function(ADD_API TARGET)

    # DX11
    if(PLATFORM_API STREQUAL "DIRECTX11")
        ADD_DX11(${TARGET})
    endif()

endfunction()
