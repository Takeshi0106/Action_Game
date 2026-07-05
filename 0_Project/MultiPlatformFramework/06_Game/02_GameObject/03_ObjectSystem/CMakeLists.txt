# ==================================================
# ObjectSystem の CMakeLists.txt
# ==================================================

# 現在の階層を取得
set(ObjectSystem_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# ヘッダー追加
target_include_directories(Game_Lib PRIVATE "${ObjectSystem_ROOT}/Include_Private")

# ソースファイル追加
target_sources(Game_Lib PRIVATE
	"${ObjectSystem_ROOT}/Src/MoveObjectSystem.cpp"
)
