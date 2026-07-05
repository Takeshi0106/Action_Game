# ==================================================
# AABBTree の CMakeLists.txt
# ==================================================

# 現在の階層を取得
set(AABBTree_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# ヘッダー追加
target_include_directories(Physics_Lib PUBLIC "${AABBTree_ROOT}/Include_Public")

# ソースファイル追加
target_sources(Physics_Lib PRIVATE
	"${AABBTree_ROOT}/Src/AABBTree.cpp"
)
