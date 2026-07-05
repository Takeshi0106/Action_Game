# ==================================================
# Light の CMakeLists.txt
# ==================================================

# 現在の階層を取得
set(Light_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# ヘッダー追加
target_include_directories(Game_Lib PRIVATE "${Light_ROOT}/Include_Private")

# ソースファイル追加
target_sources(Game_Lib PRIVATE
	"${Light_ROOT}/Src/SunLight.cpp"
)
