# ==================================================
# Object Customize の CMakeLists.txt
# ==================================================

# 現在の階層を取得
set(Customize_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# ヘッダー追加
target_include_directories(Game_Lib PRIVATE "${Customize_ROOT}/Include_Private")

# ソースファイル追加
target_sources(Game_Lib PRIVATE
    "${Customize_ROOT}/Src/Player.cpp"
	"${Customize_ROOT}/Src/Knight.cpp"
	"${Customize_ROOT}/Src/MovePlanet.cpp"
)
