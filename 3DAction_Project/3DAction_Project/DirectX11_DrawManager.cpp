
// =========================================
// ヘッダー
// =========================================
// 必須ヘッダー
#include "DirectX11_DrawManager.h" // 自分のヘッダー
// DirectXヘッダー
#include "DirectX.h" // DirectXの関数群ヘッダー
// マネージャーヘッダー
#include "ShaderManager.h" // シェーダーマネージャー
#include "ConstantBufferManager.h" // 定数バッファマネージャー
#include "VertexBufferManager.h"   // 頂点バッファマネージャー
#include "TextureManager.h" // テクスチャマネージャー
#include "ResourceViewManager.h" // ビューマネージャー
#include "SamplerManager.h" // サンプラーマネージャー
#include "IndexBufferManager.h"
// モジュール
#include "TextureLoader.h"
// シェーダータイプ
#include "UseShaderType.h"
// ログ出力
#include "ReportMessage.h"


// ============================================
// クラスのメンバー関数
// ============================================
// コンストラクタ・デストラクタ
DirectX_DrawManager::DirectX_DrawManager()
{
	// 実態作成
#if defined(DEBUG) || defined(_DEBUG)
	m_ShaderManager = std::make_unique<ShaderManager>(
		"Asset/Debug/Shader",
		"",
		"Asset/Info/ShaderReflection.txt"
	);
#else
	m_ShaderManager = std::make_unique<ShaderManager>(
		"Asset/Shader/Compile",
		"Asset/Shader/Hlsl",
		"Asset/Info/ShaderReflection.txt"
	);
#endif

	m_CBManager = std::make_unique<ConstantBufferManager>();
	m_VBManager = std::make_unique<VertexBufferManager>();
	m_TextureManager = std::make_unique<TextureManager>();
	m_ViewManager = std::make_unique<ResourceViewManager>();
	m_SamplerManager = std::make_unique<SamplerManager>();
	m_IndexBufferManager = std::make_unique<IndexBufferManager>();

	// モジュール作成
	m_TextureLoader = std::make_unique<TextureLoader>(
		m_TextureManager.get(),
		m_ViewManager.get(),
		"Asset/Texture");
}


DirectX_DrawManager::~DirectX_DrawManager()
{

}


// ==========================================
// 初期化
// ==========================================
bool DirectX_DrawManager::Init(unsigned int width, unsigned int height, HWND windowHandle)
{
	// DirectXの初期化
	if (!DirectX11::Init(width, height, windowHandle)) {
		ErrorLog::OutputToMessageBox("DirectXの初期化に失敗しました");
		return false;
	}

	// シェーダー作成
	if (!m_ShaderManager->Init(DirectX11::Get::GetDevice())) {
		ErrorLog::OutputToMessageBox("ShaderManagerの初期化に失敗しました");
		return false;
	}

	// 基本サンプラーを作成
	SamplerDesc desc = SamplerDesc::NormalSampler();
	CreateSampler(desc);

	return true;
}


// ============================================
// 後処理
// ============================================
void DirectX_DrawManager::Uninit()
{
	// メンバー変数初期化
	m_ShaderManager.reset();
	m_CBManager.reset();
	m_VBManager.reset();
	m_TextureManager.reset();
	m_ViewManager.reset();
	m_SamplerManager.reset();
	m_IndexBufferManager.reset();

	m_TextureLoader.reset();

	// DirectX11 の初期化
	DirectX11::Uninit();
}


// ===========================================
// 描画　のちにリストでスタック
// ===========================================
void DirectX_DrawManager::BegingDraw()
{
	// 描画最初の処理
	DirectX11::BeginDraw();
}

void DirectX_DrawManager::EndDraw()
{
	// 描画終了の処理
	DirectX11::EndDraw();
}

void DirectX_DrawManager::Draw(const char* drawID, const void* data, const int size)
{

}

// 描画情報を記載して描画
void DirectX_DrawManager::Draw(const char* _vsShaderName,
	const char* _psShaderName,
	const char* _textureName,
	const char* _modelName,
	const SamplerDesc& _sampler)
{
	// 描画
	DrawObject(_vsShaderName, _psShaderName, _textureName, _sampler, _modelName);
}


// ===========================================
// 頂点バッファ作成
// ===========================================
bool DirectX_DrawManager::CreateVertexBuffer(
	const char* modelName,
	const void* data,
	size_t stride,
	int vertexNumber,
	PrimitiveType type,
	BufferUsage usage,
	CPUAccess access)
{
	// 描画IDを使用して頂点数を取得


	// 頂点バッファ作成
	if (!m_VBManager->CreateVertexBuffer(
		modelName,
		DirectX11::Get::GetDevice(),
		data, // 頂点データ
		vertexNumber,   // 頂点数
		10,   // 最大頂点数
		stride,
		type,
		usage,
		access))
	{
		ErrorLog::OutputToConsole("頂点バッファ作製失敗");
		return false;
	}

	return true;
}


// ===========================================
// インデックスバッファ作成
// ===========================================
bool DirectX_DrawManager::CreateIndexBuffer(
	const char* modelName,
	const int* indexData,
	int indexNumber)
{
	// 作成
	if (m_IndexBufferManager->CreateIndexBuffer(
		modelName,
		DirectX11::Get::GetDevice(),
		indexData,
		indexNumber))
	{
		ErrorLog::OutputToConsole("インデックスバッファ作成失敗");
		return false;
	}

	return true;
}


// ===========================================
// 定数バッファ作成
// ===========================================
bool DirectX_DrawManager::CreateConstantBuffer(
	const char* constantName,
	const void* data,
	size_t size,
	BufferUsage usage,
	CPUAccess access)
{
	// 定数バッファ作成
	if (!m_CBManager->CreateConstantBuffer(
		constantName,
		DirectX11::Get::GetDevice(),
		data,
		size,
		usage,
		access))
	{
		ErrorLog::OutputToConsole("定数バッファ作製失敗");
		return false;
	}
	
	return true;
}


// =========================================
// テクスチャ作成
// =========================================
bool DirectX_DrawManager::CreateTexture(
	const char* name,
	unsigned int width,
	unsigned int height,
	Format format,
	BindFlag bindFlag,
	BufferUsage usage,
	CPUAccess cpu)
{
	// テクスチャマネージャー作成
	if (!m_TextureManager->CreateTexture(name, 
		DirectX11::Get::GetDevice(), 
		width, 
		height, 
		format, 
		bindFlag, 
		usage, 
		cpu)) 
	{
		ErrorLog::OutputToConsole("テクスチャの作成に失敗しました");
		return false;
	}

	return true;
}


// ===========================================
// サンプラー作成
// ===========================================
bool DirectX_DrawManager::CreateSampler(
	const SamplerDesc& _desc)
{
	// サンプラー作成
	if (!m_SamplerManager->CreateSampler(_desc,
		DirectX11::Get::GetDevice())) {
		ErrorLog::OutputToConsole("サンプラー作成に失敗しました");
		return false;
	}

	return true;
}


// ===========================================
// テクスチャロード
// ===========================================
bool DirectX_DrawManager::LoadTexture(const char* textureName)
{
	// テクスチャのロード関数
	if (!m_TextureLoader->ImageFileLoader(textureName, DirectX11::Get::GetDevice())){
		ErrorLog::OutputToConsole("テクスチャのロードに失敗しました");
		return false;
	}
	
	return true;
}


// ===========================================
// View作成
// ===========================================
bool DirectX_DrawManager::CreateSRV(const char* name, Format format, 
	unsigned int mostDetailedMip, unsigned int mipLevels)
{
	// リソースビュ―取得
	Texture2DData* tex = m_TextureManager->GetFindTexture2DData(name);

	if (tex == nullptr) { return false; }

	// SRV作成
	if (!m_ViewManager->CreateSRV(name,
		DirectX11::Get::GetDevice(),
		tex->GetTexture(),
		format,
		mostDetailedMip,
		mipLevels))
	{
		ErrorLog::OutputToConsole("SRVの作成に失敗しました");
		return false;
	}

	return true;
}

//bool DirectX_DrawManager::CreateUAV(const char* name, Format format, unsigned int mipSlice)
//{
//	// リソースビュ―取得
//	Texture2DData* tex = m_TextureManager->GetFindTexture2DData(name);
//
//	if (tex == nullptr) { return false; }
//
//	// UAV作成
//	if (!m_ViewManager->CreateUAV(name,
//		DirectX11::Get::GetDevice(),
//		tex->GetTexture(),
//		format,
//		mipSlice))
//	{
//		ErrorLog::OutputToConsole("UAVの作成に失敗しました");
//		return false;
//	}
//
//	return true;
//}
//
//bool DirectX_DrawManager::CreateRTV(const char* name, Format format, unsigned int mipSlice)
//{
//	// リソースビュ―取得
//	Texture2DData* tex = m_TextureManager->GetFindTexture2DData(name);
//
//	if (tex == nullptr) { return false; }
//
//	// RTV作成
//	if (!m_ViewManager->CreateRTV(name,
//		DirectX11::Get::GetDevice(),
//		tex->GetTexture(),
//		format,
//		mipSlice))
//	{
//		ErrorLog::OutputToConsole("RTVの作成に失敗しました");
//		return false;
//	}
//
//	return true;
//}
//
//bool DirectX_DrawManager::CreateDSV(const char* name, Format format, unsigned int mipSlice)
//{
//	// リソースビュ―取得
//	Texture2DData* tex = m_TextureManager->GetFindTexture2DData(name);
//
//	if (tex == nullptr) { return false; }
//
//	// DSV作成
//	if (!m_ViewManager->CreateDSV(name,
//		DirectX11::Get::GetDevice(),
//		tex->GetTexture(),
//		format,
//		mipSlice))
//	{
//		ErrorLog::OutputToConsole("DSVの作成に失敗しました");
//		return false;
//	}
//
//	return true;
//}


// ===========================================
// 定数バッファ更新
// ===========================================
void DirectX_DrawManager::UpdateShaderConstants(const char* constantName, const void* data, const int size)
{
	m_CBManager->UpdateConstantBuffer(constantName, DirectX11::Get::GetContext(), data, size);
}


// =============================================
// 頂点バッファ更新
// =============================================
void DirectX_DrawManager::UpdateVertexBuffer(const char* vertexName, const void* data, int size)
{
	m_VBManager->UpdateVertexBuffer(vertexName, DirectX11::Get::GetContext(), data, size);
}


// ===================================================
// 非公開のメンバー関数
// ===================================================
bool DirectX_DrawManager::DrawObject(
	const char* _vsShaderName, 
	const char* _psShaderName,
	const char* _textureName, 
	const SamplerDesc _sampler,
	const char* _modelName)
{
	// シェーダーバインド
	const std::vector<ConstantBufferInfo>* vsCB = m_ShaderManager->BindVertexShader(_vsShaderName,DirectX11::Get::GetContext());
	const std::vector<ConstantBufferInfo>* psCB = m_ShaderManager->BindPixelShader(_psShaderName, DirectX11::Get::GetContext());

	// 頂点バッファをバインド
	int vertexCount = m_VBManager->BindVertexBuffer(_vsShaderName, DirectX11::Get::GetContext());
	if (vertexCount == -1)
	{
		ErrorLog::OutputToConsole("頂点バッファが見つかりませんでした");
		return false;
	}

	// シェーダーの定数バッファ情報をバインド
	m_CBManager->BindConstantBuffer(vsCB, DirectX11::Get::GetContext(), VERTEXSHADER);
	m_CBManager->BindConstantBuffer(psCB, DirectX11::Get::GetContext(), PIXSELSHADER);

	// テクスチャバインド
	if (_textureName != nullptr)
	{
		// 描画コンテキスト取得
		DirectX11::Get::GetContext()->OMSetDepthStencilState(nullptr, 0);
		DirectX11::Get::GetContext()->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);

		// テクスチャ・サンプラー バインド
		m_ViewManager->BindSRV(_textureName, DirectX11::Get::GetContext(), PIXSELSHADER);
		m_SamplerManager->BindSampler(_sampler, DirectX11::Get::GetContext());
	}

	// 描画
	DirectX11::Get::GetContext()->Draw(vertexCount, 0);
	
	return true;
}
