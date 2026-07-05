# ==========================================================
# Windows 用の Math の CMakeLists.txt
# ==========================================================

# 階層を取得
set(WINDOWS_MATH_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# インクルードディレクトリーに追加
target_include_directories(Core_Lib PRIVATE "${WINDOWS_MATH_ROOT}/Include_Private")

# ソースファイルを追加
target_sources(Core_Lib PRIVATE
    "${WINDOWS_MATH_ROOT}/Src/Windows_Color.cpp"
    "${WINDOWS_MATH_ROOT}/Src/Windows_Vector2.cpp"
    "${WINDOWS_MATH_ROOT}/Src/Windows_Vector3.cpp"
    "${WINDOWS_MATH_ROOT}/Src/Windows_Vector4.cpp"
    "${WINDOWS_MATH_ROOT}/Src/Windows_Quaternion.cpp"
    "${WINDOWS_MATH_ROOT}/Src/Windows_Matrix3x3.cpp"
    "${WINDOWS_MATH_ROOT}/Src/Windows_Matrix4x4.cpp"
    "${WINDOWS_MATH_ROOT}/Src/WindowsMath_Utiles.cpp")
