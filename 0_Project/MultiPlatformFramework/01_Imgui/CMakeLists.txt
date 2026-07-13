# =========================================
# Imgui の CMakeLists.txt
# =========================================
# 現在のCmakeListの階層を取得する
set(IMGUI_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# Imgui ライブラリを作成する
add_library(IMGUI STATIC)

# ===========================================
# Imgui のコアソースファイルをビルド対象に追加
# ==========================================
set(IMGUI_CORE_SOURCES
    "${IMGUI_ROOT}/00_Core/imgui.cpp"
    "${IMGUI_ROOT}/00_Core/imgui_demo.cpp"
    "${IMGUI_ROOT}/00_Core/imgui_draw.cpp"
    "${IMGUI_ROOT}/00_Core/imgui_tables.cpp"
    "${IMGUI_ROOT}/00_Core/imgui_widgets.cpp")

# ===========================================
# Imgui のOSごとのソースファイルをビルド対象に追加
# ==========================================
# Windows 
set(IMGUI_WIN32_SOURCES
    "${IMGUI_ROOT}/01_OS/Windows/imgui_impl_win32.cpp")

# ===========================================
# Imgui のAPIごとのソースファイルをビルド対象に追加
# ===========================================
# DirectX11
set(IMGUI_DX11_SOURCES
    "${IMGUI_ROOT}/02_API/DirectX11/imgui_impl_dx11.cpp")

# OpenGL
set(IMGUI_OPENGL_SOURCES
    "${IMGUI_ROOT}/02_API/OpenGL/imgui_impl_opengl3.cpp")

# Vulkan
set(IMGUI_VULKAN_SOURCES
    "${IMGUI_ROOT}/02_API/Vulkan/imgui_impl_vulkan.cpp")

# ===========================================
# ソースファイルをビルド対象に追加
# ===========================================
# 共通のソースファイルを追加
target_sources(IMGUI PRIVATE ${IMGUI_CORE_SOURCES})

# ===========================================
# プラットフォームごとのソースファイルを追加
# ===========================================
# Windows
if(PLATFORM_OS STREQUAL "WINDOWS")
    # ビルド対象に Windows 用のソースファイルを追加
    target_sources(IMGUI PRIVATE ${IMGUI_WIN32_SOURCES})
endif()

# ===========================================
# APIごとのソースファイルを追加
# ===========================================
# DirectX11
if(PLATFORM_API STREQUAL "DIRECTX11")
    target_sources(IMGUI PRIVATE ${IMGUI_DX11_SOURCES})

# OpenGL
elseif(PLATFORM_API STREQUAL "OPENGL")
    target_sources(IMGUI PRIVATE ${IMGUI_OPENGL_SOURCES})

# Vulkan
elseif (PLATFORM_API STREQUAL "VULKAN")
    target_sources(IMGUI PRIVATE ${IMGUI_VULKAN_SOURCES})
endif()

# ===========================================
# インクルードディレクトリーを追加
# ===========================================
# 共通ソースをインクルードディレクトリーに追加
target_include_directories(IMGUI PUBLIC "${IMGUI_ROOT}/00_Core")

# OSソースをインクルードディレクトリーに追加
# Windows
if(PLATFORM_OS STREQUAL "WINDOWS")
    target_include_directories(IMGUI PUBLIC "${IMGUI_ROOT}/01_OS/Windows")
endif()

# APIソースをインクルードディレクトリーに追加
# DirectX11
if(PLATFORM_API STREQUAL "DIRECTX11")
    target_include_directories(IMGUI PUBLIC "${IMGUI_ROOT}/02_API/DirectX11")

# OpenGLB
elseif(PLATFORM_API STREQUAL "OPENGL")
    target_include_directories(IMGUI PUBLIC "${IMGUI_ROOT}/02_API/OpenGL")

# Vulkan
elseif (PLATFORM_API STREQUAL "VULKAN")
    target_include_directories(IMGUI PUBLIC "${IMGUI_ROOT}/02_API/Vulkan")
endif()
