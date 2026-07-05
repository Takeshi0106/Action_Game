# ==================================================
# Camera の CMakeLists.txt
# ==================================================

# 現在の階層を取得
set(Camera_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# ヘッダー追加
target_include_directories(Game_Lib PRIVATE "${Camera_ROOT}/Include_Private")

# ソースファイル追加
target_sources(Game_Lib PRIVATE
	"${Camera_ROOT}/Src/BaseCamera.cpp"
	"${Camera_ROOT}/Src/DCCCamera3D.cpp"
)
