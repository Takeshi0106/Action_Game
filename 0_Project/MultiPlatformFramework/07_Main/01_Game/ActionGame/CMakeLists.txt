# ==================================================
# ActionGame の CMakeLists.txt
# ==================================================

# 現在の階層を取得
set(ActionGame_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# Include追加
target_include_directories(${PROJECT_NAME} PRIVATE "${ActionGame_ROOT}/Include_Private")

# ActionGameを追加
target_sources(${PROJECT_NAME} PRIVATE 
    "${ActionGame_ROOT}/Src/ActionGame.cpp"
)
