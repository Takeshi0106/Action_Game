# ============================================
# インターフェイスDraw のCMakeLists.txt
# ============================================

add_subdirectory(Config)
add_subdirectory(Struct)

if(ARCHITECTURE_VERSION STREQUAL "1.0")
add_subdirectory(V_1.0)

elseif(ARCHITECTURE_VERSION STREQUAL "2.0")
add_subdirectory(V_2.0)

endif()
