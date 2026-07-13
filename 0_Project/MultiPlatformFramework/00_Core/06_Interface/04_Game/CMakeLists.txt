# =============================================
# InterfaceGame 用の CMakeLists.txt
# =============================================

if(ARCHITECTURE_VERSION STREQUAL "1.0")
add_subdirectory(V1.0)

elseif(ARCHITECTURE_VERSION STREQUAL "2.0")
# message(FATAL_ERROR "InterfaceGame 2.0 is not yet implemented")

endif()

