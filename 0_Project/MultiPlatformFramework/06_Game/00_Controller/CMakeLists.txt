# ==================================================
# Controller の CMakeLists.txt
# ==================================================

# 現在の階層を取得
set(Controller_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# ヘッダー追加
target_include_directories(Game_Lib PRIVATE "${Controller_ROOT}/Include_Private")

# ソースファイル追加
target_sources(Game_Lib PRIVATE
	"${Controller_ROOT}/Src/DCCCamera3D_Controller.cpp"
	"${Controller_ROOT}/Src/PlayerController.cpp"
)
