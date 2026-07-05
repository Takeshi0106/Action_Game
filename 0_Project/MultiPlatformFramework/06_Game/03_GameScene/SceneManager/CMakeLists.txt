# ==================================================
# SceneManager の CMakeLists.txt
# ==================================================

# 現在の階層を取得
set(SceneManager_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# ヘッダー追加
target_include_directories(Game_Lib PUBLIC "${SceneManager_ROOT}/Include_Public")

# ソースファイル追加
target_sources(Game_Lib PRIVATE
	"${SceneManager_ROOT}/Src/SceneManager.cpp"
)
