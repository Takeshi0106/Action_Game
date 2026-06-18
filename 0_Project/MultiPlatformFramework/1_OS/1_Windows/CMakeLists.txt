# ===========================================
# OS 用の CMakeLists.txt
# ===========================================

# デファインを追加
target_compile_definitions(
    OS_Lib PUBLIC 
UNICODE 
_UNICODE)

add_subdirectory(0_Common)

# バージョン選択
if(ARCHITECTURE_VERSION STREQUAL "1.0")
add_subdirectory(V1.0)

elseif(ARCHITECTURE_VERSION STREQUAL "2.0")
message(FATAL_ERROR "OS 2.0 is not yet implemented")

endif()
