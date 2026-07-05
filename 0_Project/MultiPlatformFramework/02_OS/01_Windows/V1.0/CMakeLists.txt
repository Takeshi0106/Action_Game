# ================================================
# WindowsSystem の CMakeLists.txt
# ================================================

set(WindowsSystem_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# インクルードディレクトリーに追加
target_include_directories(OS_Lib PUBLIC "${WindowsSystem_ROOT}/Include_Public")
# ソースファイルを追加
target_sources(OS_Lib PRIVATE 
	"${WindowsSystem_ROOT}/Src/PlatformWindowsSystem.cpp"
	"${WindowsSystem_ROOT}/Src/Windows_Input.cpp"
	"${WindowsSystem_ROOT}/Src/Windows_CursorController.cpp")
