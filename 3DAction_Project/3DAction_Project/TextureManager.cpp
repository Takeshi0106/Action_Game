
// ================================
// ヘッダー
// ================================
// 必須ヘッダー
#include "TextureManager.h"
#include "TextureData.h"
// DirectXヘッダー
#include <d3d11.h>        // DirectXのAPIヘッダー
// バッファユーリアリティヘッダー
#include "Windows/DirectX11/DirectX11_FormatConverter.h"
// ログ出力
#include "ReportMessage.h"


#if defined(DEBUG) || defined(_DEBUG)
// デバッグ用
#pragma comment(lib, "dxguid.lib")
// バイナリーデータヘッダー
#include "BinaryView.h"
#endif


// ================================
// 前方宣言
// ================================
UINT ToDXBindFlag(BindFlag flags); // バインドフラグをDirectXように変換


// =================================
// メンバー関数
// =================================

// ======================================
// スワップチェインからテクスチャを作成
// ======================================
// スワップチェインからテクスチャを作成
bool TextureManager::CreateTextureFromSwapChain(
    const Hashed_String& name,
    IDXGISwapChain* swapChain)
{
    // 情報を構造体にまとめてマップに保存
    auto textureData = std::make_unique<SwapchainTextureData>();

	// スワップチェインからテクスチャ作成
    if (!textureData->CreateTextureFromSwapChain(swapChain))
    {
        DebugLog::OutputToConsole(u8"スワップチェインからテクスチャの作成に失敗しました " + name.GetString());
        return false;
	}

	// 配列に代入
	m_Textures[name] = std::move(textureData);

#if defined(DEBUG) || defined(_DEBUG)
    DebugLog::OutputToConsole(u8"テクスチャバッファ " + name.GetString() + u8" を作成しました");

    // 名前を設定
    m_Textures[name]->GetTexture()->SetPrivateData(
        WKPDID_D3DDebugObjectName,
        UINT(name.GetString().GetBinaryView().GetSize()),
        name.GetString().GetBinaryView().GetData());
#endif

    return true;
}


// ======================================
// テクスチャを作成
// ======================================
bool TextureManager::CreateTexture(
    const Hashed_String& name,
    ID3D11Device* device,
    unsigned int width,
    unsigned int height,
    Format format,
	BindFlag bindFlag,
    BufferUsage usage,
    CPUAccess flag,
    TextureInitData* initData)
{
	// すでに存在しているか確認
    if (m_Textures.find(name) != m_Textures.end()) {
        // すでに存在している
        WarningLog::OutputToConsole(u8"テクスチャ : " + name.GetString() + u8" はすでに存在しています");
        return false;
    }

	// 情報を構造体にまとめてマップに保存
	auto textureData = std::make_unique<BindableTextureData>();

    if (initData != nullptr)
    {
        // 初期化データあり
        D3D11_SUBRESOURCE_DATA dxInitData = {};
        dxInitData.pSysMem = initData->data;
        dxInitData.SysMemPitch = static_cast<UINT>(initData->rowPitch);
        dxInitData.SysMemSlicePitch = static_cast<UINT>(initData->slicePitch);

        // テクスチャ作成
        if (!textureData->CreateTexture2D(
            device,
            width,
            height,
            DirectX11_FormatConverter::ToDXFormat(format),
            D3D11_BIND_FLAG(ToDXBindFlag(bindFlag)),
            DirectX11_FormatConverter::ToDXUsage(usage),
            D3D11_CPU_ACCESS_FLAG(DirectX11_FormatConverter::ToDXCPUAccess(flag)),
            &dxInitData))
        {
            DebugLog::OutputToConsole(u8"テクスチャの作成に失敗しました " + name.GetString());
            return false;
        }
    }
    else
    {
        // 初期化データなし
        // テクスチャ作成
        if (!textureData->CreateTexture2D(
            device,
            width,
            height,
            DirectX11_FormatConverter::ToDXFormat(format),
            D3D11_BIND_FLAG(ToDXBindFlag(bindFlag)),
            DirectX11_FormatConverter::ToDXUsage(usage),
            D3D11_CPU_ACCESS_FLAG(DirectX11_FormatConverter::ToDXCPUAccess(flag))))
        {
            DebugLog::OutputToConsole(u8"テクスチャの作成に失敗しました " + name.GetString());
            return false;
        }
    }

	// 配列に代入
	m_Textures[name] = std::move(textureData);

#if defined(DEBUG) || defined(_DEBUG)
    DebugLog::OutputToConsole(u8"テクスチャバッファ " + name.GetString() + u8" を作成しました");

    // 名前を設定
    m_Textures[name]->GetTexture()->SetPrivateData(
        WKPDID_D3DDebugObjectName,
        UINT(name.GetString().GetBinaryView().GetSize()),
        name.GetString().GetBinaryView().GetData());
#endif

	return true;
}

// ======================================
// テクスチャ２Dデータを探してポインターを返す
// ======================================
Texture2DData* TextureManager::GetFindTexture2DData(const Hashed_String& name)
{
    // 探す
    auto it = m_Textures.find(name);

    if (it != m_Textures.end())
    {
        // 定数バッファを返す
        return it->second.get();
    }
    else {
        ErrorLog::OutputToConsole(u8" テクスチャ : " + name.GetString() + u8" が見つかりませんでした");
    }

    return nullptr;
}


// ========================================
// テクスチャが存在するか
// ========================================
bool TextureManager::IsExistTexture(const Hashed_String& name)
{
    // 探す
    auto it = m_Textures.find(name);
    if (it != m_Textures.end())
    {
        // 存在する
        return true;
    }
    // 存在しない
    return false;
}   


// ========================================
// テクスチャをすべて削除
// ========================================
void TextureManager::ReleaseAllTexture()
{
    m_Textures.clear();
}


// =========================================
// バインドフラグ変換
// =========================================
UINT ToDXBindFlag(BindFlag flags)
{
    UINT result = 0;

    if (static_cast<unsigned int>(flags) & static_cast<unsigned int>(BindFlag::Bind_VertexBuffer))
    {
        result |= D3D11_BIND_VERTEX_BUFFER;
    }
    if (static_cast<unsigned int>(flags) & static_cast<unsigned int>(BindFlag::Bind_IndexBuffer))
    {
        result |= D3D11_BIND_INDEX_BUFFER;
    }
    if (static_cast<unsigned int>(flags) & static_cast<unsigned int>(BindFlag::Bind_ConstantBuffer))
    {
        result |= D3D11_BIND_CONSTANT_BUFFER;
    }
    if (static_cast<unsigned int>(flags) & static_cast<unsigned int>(BindFlag::Bind_ShaderResource))
    {
        result |= D3D11_BIND_SHADER_RESOURCE;
    }
    if (static_cast<unsigned int>(flags) & static_cast<unsigned int>(BindFlag::Bind_RenderTarget))
    {
        result |= D3D11_BIND_RENDER_TARGET;
    }
    if (static_cast<unsigned int>(flags) & static_cast<unsigned int>(BindFlag::Bind_DepthStencil))
    {
        result |= D3D11_BIND_DEPTH_STENCIL;
    }
    if (static_cast<unsigned int>(flags) & static_cast<unsigned int>(BindFlag::Bind_UnorderedAccess))
    {
        result |= D3D11_BIND_UNORDERED_ACCESS;
    }

    return result;
}
