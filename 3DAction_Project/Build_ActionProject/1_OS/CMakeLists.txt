# ===========================================
# OS 用の CMakeLists.txt
# ===========================================

# Windows
if(PLATFORM_OS STREQUAL "WINDOWS")
    add_subdirectory(Windows)
    add_subdirectory(0_PCOS)
endif()

# 関数定義
function(ADD_OS TARGET)

    # Windows 
    if(PLATFORM_OS STREQUAL "WINDOWS")
        ADD_PCOS(${TARGET})
        ADD_WINDOWS(${TARGET})
    endif()

endfunction()
