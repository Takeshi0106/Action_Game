# ==================================================
# CustomizeScene の CMakeLists.txt
# ==================================================

# 現在の階層を取得
set(CustomizeScene_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# ヘッダー追加
target_include_directories(Game_Lib PRIVATE "${CustomizeScene_ROOT}/Include_Private")

# ソースファイル追加
target_sources(Game_Lib PRIVATE
	"${CustomizeScene_ROOT}/Src/DebugSceneState.cpp"
	"${CustomizeScene_ROOT}/Src/TitleSceneState.cpp"
	"${CustomizeScene_ROOT}/Src/GameMainState.cpp"
	"${CustomizeScene_ROOT}/Src/ResultSceneState.cpp"
)
