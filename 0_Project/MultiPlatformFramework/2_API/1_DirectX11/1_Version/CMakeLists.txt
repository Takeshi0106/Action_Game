# ===========================================
# OS 用の CMakeLists.txt
# ===========================================

if(ARCHITECTURE_VERSION STREQUAL "1.0")
add_subdirectory(V1.0)

elseif(ARCHITECTURE_VERSION STREQUAL "2.0")
add_subdirectory(V2.0)

endif()
