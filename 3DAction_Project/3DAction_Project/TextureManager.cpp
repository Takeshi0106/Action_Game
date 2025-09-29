
// ================================
// ヘッダー
// ================================
// 必須ヘッダー
#include "TextureManager.h"
#include "TextureData.h"
// DirectXヘッダー
#include <d3d11.h>        // DirectXのAPIヘッダー
#include <wrl/client.h>   // スマートポインター
// 
#include "DirectX_BufferUtils.h"


// ================================
// 静的メンバー配列
// ================================
std::unordered_map<std::string, std::unique_ptr<Texture2DData>>TextureManager::m_Textures;


// ================================
// 前方宣言
// ================================
UINT ToDXBindFlag(BindFlag flags); // バインドフラグをDirectXように変換


// =================================
// メンバー関数
// =================================
bool TextureManager::CreateTexture(
    const std::string& name,
    ID3D11Device* device,
    unsigned int width,
    unsigned int height,
    Format format,
	BindFlag bindFlag,
    BufferUsage usage,
    CPUAccess flag)
{
	// 情報を構造体にまとめてマップに保存
	auto textureData = std::make_unique<Texture2DData>();

	// テクスチャ作成
	if (!textureData->CreateTexture2D(
		device,
		width,
		height,
		BufferUtils::toDXFormat(format),
		D3D11_BIND_FLAG(ToDXBindFlag(bindFlag)),
		BufferUtils::ToDXUsage(usage),
		D3D11_CPU_ACCESS_FLAG(BufferUtils::ToDXCPUAccess(flag))))
	{
		DebugLog::OutputToConsole(("テクスチャの作成に失敗しました " + name).c_str());
		return false;
	}

	// 配列に代入
	m_Textures[name] = std::move(textureData);

	// 名前を保存しておく
	m_Logger.Log(name.c_str());

	return true;
}


// バインドフラグ変換
UINT ToDXBindFlag(BindFlag flags)
{
    UINT result = 0;

    if (static_cast<unsigned int>(flags) & static_cast<unsigned int>(BindFlag::VertexBuffer))
    {
        result |= D3D11_BIND_VERTEX_BUFFER;
    }
    if (static_cast<unsigned int>(flags) & static_cast<unsigned int>(BindFlag::IndexBuffer))
    {
        result |= D3D11_BIND_INDEX_BUFFER;
    }
    if (static_cast<unsigned int>(flags) & static_cast<unsigned int>(BindFlag::ConstantBuffer))
    {
        result |= D3D11_BIND_CONSTANT_BUFFER;
    }
    if (static_cast<unsigned int>(flags) & static_cast<unsigned int>(BindFlag::ShaderResource))
    {
        result |= D3D11_BIND_SHADER_RESOURCE;
    }
    if (static_cast<unsigned int>(flags) & static_cast<unsigned int>(BindFlag::RenderTarget))
    {
        result |= D3D11_BIND_RENDER_TARGET;
    }
    if (static_cast<unsigned int>(flags) & static_cast<unsigned int>(BindFlag::DepthStencil))
    {
        result |= D3D11_BIND_DEPTH_STENCIL;
    }
    if (static_cast<unsigned int>(flags) & static_cast<unsigned int>(BindFlag::UnorderedAccess))
    {
        result |= D3D11_BIND_UNORDERED_ACCESS;
    }

    return result;
}