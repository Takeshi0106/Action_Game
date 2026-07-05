# ==================================================
# FadeManager の CMakeLists.txt
# ==================================================

# 現在の階層を取得
set(FadeManager_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# ヘッダー追加
target_include_directories(Game_Lib PUBLIC "${FadeManager_ROOT}/Include_Public")

# ソースファイル追加
target_sources(Game_Lib PRIVATE
	"${FadeManager_ROOT}/Src/FadeManager.cpp"
)
