# ==================================================
# 2DTriangle の CMakeLists.txt
# ==================================================

# 現在の階層を取得
set(2DTriangle_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# ヘッダー追加
target_include_directories(Game_Lib PRIVATE "${2DTriangle_ROOT}/Include_Private")

# ソースファイル追加
target_sources(Game_Lib PRIVATE
	"${2DTriangle_ROOT}/Src/2DTriangle.cpp"
)
