
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
// モジュール
#include "TextureLoader.h"
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
	const char* drawID,
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
		drawID,
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

bool DirectX_DrawManager::CreateUAV(const char* name, Format format, unsigned int mipSlice)
{
	// リソースビュ―取得
	Texture2DData* tex = m_TextureManager->GetFindTexture2DData(name);

	if (tex == nullptr) { return false; }

	// UAV作成
	if (!m_ViewManager->CreateUAV(name,
		DirectX11::Get::GetDevice(),
		tex->GetTexture(),
		format,
		mipSlice))
	{
		ErrorLog::OutputToConsole("UAVの作成に失敗しました");
		return false;
	}

	return true;
}

bool DirectX_DrawManager::CreateRTV(const char* name, Format format, unsigned int mipSlice)
{
	// リソースビュ―取得
	Texture2DData* tex = m_TextureManager->GetFindTexture2DData(name);

	if (tex == nullptr) { return false; }

	// RTV作成
	if (!m_ViewManager->CreateRTV(name,
		DirectX11::Get::GetDevice(),
		tex->GetTexture(),
		format,
		mipSlice))
	{
		ErrorLog::OutputToConsole("RTVの作成に失敗しました");
		return false;
	}

	return true;
}

bool DirectX_DrawManager::CreateDSV(const char* name, Format format, unsigned int mipSlice)
{
	// リソースビュ―取得
	Texture2DData* tex = m_TextureManager->GetFindTexture2DData(name);

	if (tex == nullptr) { return false; }

	// DSV作成
	if (!m_ViewManager->CreateDSV(name,
		DirectX11::Get::GetDevice(),
		tex->GetTexture(),
		format,
		mipSlice))
	{
		ErrorLog::OutputToConsole("DSVの作成に失敗しました");
		return false;
	}

	return true;
}


// ===========================================
// 定数バッファ更新
// ===========================================
void DirectX_DrawManager::UpdateShaderConstants(const char* constantName, const void* data, const int size)
{
	// 定数バッファデータを取得
	ConstantBufferData* cbData = m_CBManager->GetFindConstantBuffer(constantName);

	// 定数バッファ更新
	cbData->UpdateConstantBuffer(DirectX11::Get::GetContext(), data, size);
}


// =============================================
// 頂点バッファ更新
// =============================================
void DirectX_DrawManager::UpdateVertexBuffer(const char* vertexName, const void* data, int size)
{
	// 頂点バッファデータ取得
	VertexBufferData* vbData = m_VBManager->GetFindVertexData(vertexName);

	// 頂点バッファ更新
	vbData->UpdateBuffer(DirectX11::Get::GetContext(), data, size);
}


// ===================================================
// 非公開のメンバー関数
// ===================================================
void DirectX_DrawManager::DrawObject(const char* _vsShaderName, 
	const char* _psShaderName,
	const char* _textureName, 
	const SamplerDesc _desc,
	const char* _modelName)
{
	// Stringに変換
	std::string vsName = _vsShaderName;
	std::string psName = _psShaderName;

	// シェーダー取得
	VertexShaderData* vs = m_ShaderManager->GetFindVertexShader(vsName);
	PixelShaderData* ps = m_ShaderManager->GetFindPixelShader(psName);

	// 入力レイアウトを設定
	DirectX11::Get::GetContext()->IASetInputLayout(vs->GetInputLayout());

	// 頂点バッファを取得
	VertexBufferData* vertexBufferData = m_VBManager->GetFindVertexData(vsName);

	// 入力アセンブラ
	ID3D11Buffer* vbuffers = vertexBufferData->GetVertexBuffer();
	UINT stride = UINT(vertexBufferData->GetStride());
	UINT offset = 0;

	// 頂点バッファをセット
	DirectX11::Get::GetContext()->IASetVertexBuffers(0, 1, &vbuffers, &stride, &offset);
	vertexBufferData->SetIsUpdate(false);

	// トポロギー設定
	DirectX11::Get::GetContext()->IASetPrimitiveTopology(vertexBufferData->GetPrimitiveType());

	// 定数バッファ情報を取得
	std::vector<ConstantBufferInfo> cbInfo = vs->GetCBInfo();
	std::vector<ID3D11Buffer*> buffers(cbInfo.size(), nullptr);

	// 頂点シェーダーの定数バッファバインド設定
	for (size_t i = 0; i < cbInfo.size(); i++)
	{
		// 定数バッファ取得
		ConstantBufferData* buffer = m_CBManager->GetFindConstantBuffer(cbInfo[i].GetName());
		if (buffer) {
			// VSスロット番号にバインド
			buffers[i] = buffer->GetBuffer();
		}
	}

	// 定数バッファをまとめてバインド
	DirectX11::Get::GetContext()->VSSetConstantBuffers(
		0,                         // 先頭スロット
		static_cast<UINT>(buffers.size()),
		buffers.data()             // 配列を渡す
	);

	// ピクセルシェーダーの定数バッファバインド設定
	cbInfo = ps->GetCBInfo();
	buffers.resize(cbInfo.size(), nullptr);

	for (size_t i = 0; i < cbInfo.size(); i++)
	{
		// 定数バッファ取得
		ConstantBufferData* buffer = m_CBManager->GetFindConstantBuffer(cbInfo[i].GetName());
		if (buffer) {
			// PSスロット番号にバインド
			buffers[i] = buffer->GetBuffer();
		}
	}

	// まとめて定数バッファをバインド
	DirectX11::Get::GetContext()->PSSetConstantBuffers(
		0,                         // 先頭スロット
		static_cast<UINT>(buffers.size()),
		buffers.data()             // 配列を渡す
	);

	// テクスチャバインド
	if (_textureName != nullptr)
	{
		// 描画コンテキスト取得
		DirectX11::Get::GetContext()->OMSetDepthStencilState(nullptr, 0);
		DirectX11::Get::GetContext()->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);

		// 取得
		ID3D11ShaderResourceView* srv = m_ViewManager->GetFindSRV(_textureName)->GetSRV();
		ID3D11SamplerState* sampler = m_SamplerManager->GetSampler(_desc)->GetSampler();

		// テクスチャ・サンプラー バインド
		DirectX11::Get::GetContext()->PSSetShaderResources(0, 1, &srv);
		DirectX11::Get::GetContext()->PSSetSamplers(0, 1, &sampler);
	}

	// シェーダーセット
	DirectX11::Get::GetContext()->VSSetShader(vs->GetVertexShader(), nullptr, 0);
	DirectX11::Get::GetContext()->PSSetShader(ps->GetPixelShader(), nullptr, 0);

	// 描画
	DirectX11::Get::GetContext()->Draw(vertexBufferData->GetVertexCount(), 0);
}