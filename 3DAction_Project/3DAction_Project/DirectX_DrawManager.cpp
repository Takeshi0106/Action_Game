
// =========================================
// ヘッダー
// =========================================
// 必須ヘッダー
#include "DirectX_DrawManager.h" // 自分のヘッダー
// DirectXヘッダー
#include "DirectX.h" // DirectXの関数群ヘッダー
// マネージャーヘッダー
#include "ShaderManager.h" // シェーダーマネージャー
#include "ConstantBufferManager.h" // 定数バッファマネージャー
// ログ出力
#include "ReportMessage.h"
// デバッグ用
#include "Timer.h"


// ==========================================
// 静的メンバー変数
// ==========================================
#if defined(DEBUG) || defined(_DEBUG)

ShaderManager DirectX_DrawManager::m_ShaderManager = {
    "Debug/Log/Shader.txt",           // 使用したシェイダーの名前を書き出すログのパス
    "Asset/Debug/Shader",             // デバッグ時のコンパイルしたシェイダーを入れるパス
    "",                               // Debug時には使用しないパス　(.hlslがある場所を示すパス)
    "Asset/Info/ShaderReflection.txt" // リフレクションした情報を出力するファイルパス
};

#else

ShaderManager DirectX_DrawManager::m_ShaderManager = {
    "Debug/Log/Shader.txt",             // 使用したシェイダーの名前を書き出すログのパス
    "Asset/Shader/Compile",             // デバッグ時のコンパイルしたシェイダーを入れるパス
    "Asset/Shader/Hlsl",                // Debug時には使用しないパス　(.hlslがある場所を示すパス)
    "Asset/Info/ShaderReflection.txt"   // リフレクションした情報を出力するファイルパス
};

#endif

ConstantBufferManager DirectX_DrawManager::m_CBManager = {
    "Debug/Log/ConstantBuffers.txt"    // 使用したコンスタンスバッファの名前を書き出すパス
};


// ==========================================
// 初期化
// ==========================================
bool DirectX_DrawManager::Init(unsigned int width, unsigned int height, HWND windowHandle)
{
    // DirectXの初期化
    if (!DirectX11::Init(width, height, windowHandle)) {
        ErrorLog::OutputToMessageBox("DirectXの初期化に失敗しました");
        return false; // 失敗したら戻る
    }

    // シェーダー・定数バッファ作成
    if (!m_ShaderManager.Init(DirectX11::Get::GetDevice(), m_CBManager)) {
        ErrorLog::OutputToMessageBox("ShaderManagerの初期化に失敗しました");
        return false; // 失敗したら戻る
    }

    Timer::Init(); // タイマー初期化
    Timer::Start(); // タイマー開始

    return true;
}


// ============================================
// 後処理
// ============================================
void DirectX_DrawManager::Uninit()
{
    DirectX11::Uninit();      // Directの後処理
    m_ShaderManager.Uninit(); // シェーダ―マネージャーの後処理
}


// ===========================================
// デバッグ用描画
// ===========================================
void DirectX_DrawManager::DebugDraw()
{
    float a[4] = { 1.0f,0.1f,1.0f,0.1f };

    Timer::Debug_CheckUpdate(); // タイマーデバッグ

    // デバッグ時 
    DirectX11::BeginDraw(); // 描画の開始処理

    // m_ConstantBufferManager.BindVS("Transform", DirectX11::Get::GetContext());
    // m_ConstantBufferManager.BindVS("DebugParams", DirectX11::Get::GetContext());
    // m_ConstantBufferManager.UpdateConstantBuffer("PSInput", a, sizeof(a), DirectX11::Get::GetContext());
    // m_ShaderManager.BindVertexShaderSet("VS_Debug", DirectX11::Get::GetContext());
    // m_ShaderManager.BindPixelShaderSet("PS_Debug", DirectX11::Get::GetContext());

    DirectX11::DebugDraw(Timer::GetElapsedTime()); // デバッグ表示

    DirectX11::EndDraw(); // 描画の終わり処理

    Timer::LastUpdate(); // タイマー更新処理
}