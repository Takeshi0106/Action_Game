
// =========================================
// ヘッダー
// =========================================
// 必須ヘッダー
#include "DirectX11_DrawManager.h" // 自分のヘッダー
// DirectXヘッダー
#include "DirectX.h" // DirectXの関数群ヘッダー
// シェーダータイプ
#include "UseShaderType.h"
// ログ出力
#include "ReportMessage.h"


// ============================================
// クラスのメンバー関数
// ============================================
// コンストラクタ・デストラクタ
DirectX_DrawManager::DirectX_DrawManager(const DrawPathConfig& _config) :
	m_ShaderManager(
		_config.shaderBinaryPath,
		_config.shaderSourcePath,
		_config.shaderReflectionPath
	),
	m_TextureLoader(
		&m_TextureManager,
		&m_ViewManager,
		_config.texturePath
	),
	m_ModelConversionModule(
		_config.objModelPath,
		"Asset/SelfModel"
	)
{
}


DirectX_DrawManager::~DirectX_DrawManager()
{

}


// ==========================================
// 初期化
// ==========================================
bool DirectX_DrawManager::Init(uint16_t width, uint16_t height, HWND windowHandle)
{
	// 画面サイズ保存
	m_ScreenWidth = width;
	m_ScreenHeight = height;

	// DirectXの初期化
	if (!DirectX11::Init(width, height, windowHandle)) {
		ErrorLog::OutputToMessageBox("DirectXの初期化に失敗しました");
		return false;
	}

	// スワップチェインからテクスチャを作成
	if (!m_TextureManager.CreateTextureFromSwapChain(kFinalRTName, DirectX11::Get::GetSwapChain())) {
		ErrorLog::OutputToConsole("スワップチェインからテクスチャの作成に失敗しました");
		return false;
	}

	// RTV作成
	if (!CreateRTV(kFinalRTName, 0))
	{
		ErrorLog::OutputToConsole("スワップチェインからRTVの作成に失敗しました");
		return false;
	}

	// 深度バッファ作成
	if(!CreateTexture(
		kFInalDSName,
		width,
		height,
		Format::Format_D24_UNorm_S8_UInt,
		BindFlag::Bind_DepthStencil,
		BufferUsage::Default,
		CPUAccess::None))
	{
		ErrorLog::OutputToConsole("基本深度バッファの作成に失敗しました");
		return false;
	}

	// DSV
	if (!m_ViewManager.CreateDSV(
		kFInalDSName,
		DirectX11::Get::GetDevice(),
		m_TextureManager.GetFindTexture2DData(kFInalDSName)->GetTexture(),
		Format::Format_D24_UNorm_S8_UInt)) {
		ErrorLog::OutputToConsole("基本DSVの作成に失敗しました");
		return false;
	}

	if (!m_ShaderManager.Init(DirectX11::Get::GetDevice())) {
		ErrorLog::OutputToMessageBox("ShaderManagerの初期化に失敗しました");
		return false;
	}

	// モデルマネージャー初期化
	if (!m_ModelManager.Init(*this)) {
		ErrorLog::OutputToMessageBox("ModelManagerの初期化に失敗しました");
		return false;
	}

	// 基本サンプラーを作成
	SamplerDesc desc = SamplerDesc::NormalSampler();
	CreateSampler(desc);

	//　RTとDSをバインド
	BindRenderTarget();

	return true;
}


// ============================================
// 後処理
// ============================================
void DirectX_DrawManager::Uninit()
{
	// DirectX11 の初期化
	DirectX11::Uninit();
}


// ===========================================
// 描画　のちにリストでスタック
// ===========================================
void DirectX_DrawManager::BegingDraw()
{
	// RTV を取得
	RTVData* rtv = m_ViewManager.GetRTV(kFinalRTName);
	DSVData* dsv = m_ViewManager.GetDSV(kFInalDSName);

	// クリアカラー配列作成
	float clearColor[4] = {
		kClearColor.r,
		kClearColor.g,
		kClearColor.b,
		kClearColor.a
	};

	// 初期化
	DirectX11::Get::GetContext()->ClearRenderTargetView(rtv->GetRTV(), clearColor);
	DirectX11::Get::GetContext()->ClearDepthStencilView(dsv->GetDSV(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// バインド
	BindRenderTarget();
}

void DirectX_DrawManager::EndDraw()
{
	// 描画終了の処理
	DirectX11::Get::GetSwapChain()->Present(1, 0);
}

// モデル描画
void DirectX_DrawManager::ModelDraw(const char* _vsShaderName, 
	const char* _psShaderName, 
	const char* _modelName)
{
	DrawModelObject(_vsShaderName, _psShaderName, _modelName);
}

// インデックスバッファを使用したメッシュ描画
void DirectX_DrawManager::IndexedDraw(
	const char* _vsShaderName,
	const char* _psShaderName,
	const char* _vbName,
	const char* _ibName,
	const char* _textureName,
	const SamplerDesc& _sampler)
{
	DrawIndexObject(_vsShaderName, _psShaderName, _vbName, _ibName, _textureName, _sampler);
}

// 描画情報を記載して描画
void DirectX_DrawManager::PrimitiveDraw(const char* _vsShaderName,
	const char* _psShaderName,
	const char* _vertexName,
	const char* _textureName,
	const SamplerDesc& _sampler)
{
	// 描画
	DrawPrimitiveObject(_vsShaderName, _psShaderName, _vertexName, _textureName, _sampler);
}


// ===========================================
// 頂点バッファ作成
// ===========================================
bool DirectX_DrawManager::CreateVertexBuffer(
	const char* modelName,
	const void* data,
	size_t stride,
	uint32_t vertexNumber,
	uint32_t maxNumber,
	PrimitiveType type,
	BufferUsage usage,
	CPUAccess access)
{
	// 描画IDを使用して頂点数を取得


	// 頂点バッファ作成
	if (!m_VBManager.CreateVertexBuffer(
		modelName,
		DirectX11::Get::GetDevice(),
		data, // 頂点データ
		vertexNumber,   // 頂点数
		maxNumber,   // 最大頂点数
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
	const uint32_t* indexData,
	uint32_t indexNumber)
{
	// 作成
	if (!m_IndexBufferManager.CreateIndexBuffer(
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
	if (!m_CBManager.CreateConstantBuffer(
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
	uint32_t width,
	uint32_t height,
	Format format,
	BindFlag bindFlag,
	BufferUsage usage,
	CPUAccess cpu)
{
	// テクスチャマネージャー作成
	if (!m_TextureManager.CreateTexture(name, 
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
	if (!m_SamplerManager.CreateSampler(_desc,
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
	if (!m_TextureLoader.ImageFileLoader(textureName, DirectX11::Get::GetDevice())) {
		ErrorLog::OutputToConsole("テクスチャのロードに失敗しました");
		return false;
	}

	return true;
}


// ===========================================
// モデルロード
// ===========================================
bool DirectX_DrawManager::LoadModel(const char* modelName, const char* modelFolderName)
{
	// モデル変換モジュールを使用してモデルをロード
	if (!m_ModelConversionModule.LoadAndRegisterModelResources(
		modelName,
		*this,
		m_ModelManager,
		modelFolderName))
	{
		ErrorLog::OutputToConsole("モデルのロードに失敗しました");
		return false;
	}
	return true;
}


// ===========================================
// View作成
// ===========================================
// SRV作成
bool DirectX_DrawManager::CreateSRV(const char* name, Format format, 
	unsigned int mostDetailedMip, unsigned int mipLevels)
{
	// リソースビュ―取得
	Texture2DData* tex = m_TextureManager.GetFindTexture2DData(name);

	if (tex == nullptr) { return false; }

	// SRV作成
	if (!m_ViewManager.CreateSRV(name,
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

// RTV作成
bool DirectX_DrawManager::CreateRTV(const char* name, uint32_t mipSlice)
{
	// リソースビュ―取得
	Texture2DData* tex = m_TextureManager.GetFindTexture2DData(name);

	if (tex == nullptr) { return false; }

	// RTV作成
	if (!m_ViewManager.CreateRTV(name,
		DirectX11::Get::GetDevice(),
		tex->GetTexture(),
		mipSlice))
	{
		ErrorLog::OutputToConsole("RTVの作成に失敗しました");
		return false;
	}

	return true;
}

// DSV作成
bool DirectX_DrawManager::CreateDSV(const char* name, Format format)
{
	// リソースビュ―取得
	Texture2DData* tex = m_TextureManager.GetFindTexture2DData(name);

	if (tex == nullptr) { return false; }

	// DSV作成
	if (!m_ViewManager.CreateDSV(name,
		DirectX11::Get::GetDevice(),
		tex->GetTexture(),
		format))
	{
		ErrorLog::OutputToConsole("DSVの作成に失敗しました");
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


// ===========================================
// 定数バッファ更新
// ===========================================
void DirectX_DrawManager::UpdateShaderConstants(const char* constantName, const void* data, const int size)
{
	m_CBManager.UpdateConstantBuffer(constantName, DirectX11::Get::GetContext(), data, size);
}


// =============================================
// 頂点バッファ更新
// =============================================
void DirectX_DrawManager::UpdateVertexBuffer(const char* vertexName, const void* data, int size)
{
	m_VBManager.UpdateVertexBuffer(vertexName, DirectX11::Get::GetContext(), data, size);
}


// =============================================
// レンダーターゲットバインド
// =============================================
void DirectX_DrawManager::BindRenderTarget(const char* rtvName, const char* dsvName)
{
	// RTV・DSV取得
	RTVData* rtv;
	DSVData* dsv;

	// nullptr だったら最終描画用をセット
	if (rtvName == nullptr)
	{
		rtv = m_ViewManager.GetRTV(kFinalRTName);
	}
	else
	{
		rtv = m_ViewManager.GetRTV(rtvName);
	}

	if (dsvName == nullptr)
	{
		dsv = m_ViewManager.GetDSV(kFInalDSName);
	}
	else
	{
		dsv = m_ViewManager.GetDSV(dsvName);
	}

	// セット
	DirectX11::Get::GetContext()->OMSetRenderTargets(1, rtv->GetRTVAddress(), dsv->GetDSV());

	// ビューポート設定
	DirectX11::SetViewPort(rtv->GetWidth(), rtv->GetHeight());
}


// ===================================================
// 描画設定(カリング、塗り)
// ===================================================
void DirectX_DrawManager::SetDrawSetting(FillModeSetting fillMode, CullingSetting culling)
{
	DirectX11::SetDrawSetting(culling, fillMode);
}


// ===================================================
// 深度ステンシル設定
// ===================================================
void DirectX_DrawManager::SetDepthStencilSetting(DepthStencilSetting depthStencil)
{
	DirectX11::SetDepthStencilSetting(depthStencil);
}


// ===================================================
// アルファディザ設定
// ===================================================
void DirectX_DrawManager::SetAlphaDizaSetting(AlphaDizaSetting alphaDiza)
{
	DirectX11::SetAlphaDizaSetting(alphaDiza);
}


// ===================================================
// ゲッター
// ===================================================
// デバイス取得
ID3D11Device* DirectX_DrawManager::GetDevice()
{
	return DirectX11::Get::GetDevice();
}

// デバイスコンテキスト取得
ID3D11DeviceContext* DirectX_DrawManager::GetDeviceContext()
{
	return DirectX11::Get::GetContext();
}


// ===================================================
// 自作メッシュを描画
// ===================================================
bool DirectX_DrawManager::DrawPrimitiveObject(
	const char* _vsShaderName, 
	const char* _psShaderName,
	const char* _modelName,
	const char* _textureName, 
	const SamplerDesc _sampler)
{
	// シェーダーバインド
	const std::vector<ConstantBufferInfo>* vsCB = m_ShaderManager.BindVertexShader(_vsShaderName,DirectX11::Get::GetContext());
	const std::vector<ConstantBufferInfo>* psCB = m_ShaderManager.BindPixelShader(_psShaderName, DirectX11::Get::GetContext());

	// 頂点バッファをバインド
	int vertexCount = m_VBManager.BindVertexBuffer(_modelName, DirectX11::Get::GetContext());
	if (vertexCount == -1)
	{
		ErrorLog::OutputToConsole("頂点バッファが見つかりませんでした");
		return false;
	}

	// シェーダーの定数バッファ情報をバインド
	m_CBManager.BindConstantBuffer(vsCB, DirectX11::Get::GetContext(), VERTEXSHADER);
	m_CBManager.BindConstantBuffer(psCB, DirectX11::Get::GetContext(), PIXSELSHADER);

	// テクスチャバインド
	if (_textureName != nullptr)
	{
		// テクスチャ・サンプラー バインド
		m_ViewManager.BindSRV(_textureName, DirectX11::Get::GetContext(), PIXSELSHADER);
		m_SamplerManager.BindSampler(_sampler, DirectX11::Get::GetContext());
	}

	// 描画
	DirectX11::Get::GetContext()->Draw(vertexCount, 0);
	
	return true;
}

// ===================================================
// インデックスバッファを使用したメッシュ描画
// ===================================================
bool DirectX_DrawManager::DrawIndexObject(
	const char* _vsShaderName,
	const char* _psShaderName,
	const char* _vbName,
	const char* _ibName,
	const char* _textureName,
	const SamplerDesc _sampler)
{
	// シェーダーバインド
	const std::vector<ConstantBufferInfo>* vsCB = m_ShaderManager.BindVertexShader(_vsShaderName, DirectX11::Get::GetContext());
	const std::vector<ConstantBufferInfo>* psCB = m_ShaderManager.BindPixelShader(_psShaderName, DirectX11::Get::GetContext());

	// 頂点バッファをバインド
	int vertexCount = m_VBManager.BindVertexBuffer(_vbName, DirectX11::Get::GetContext());
	if (vertexCount == -1)
	{
		ErrorLog::OutputToConsole("頂点バッファが見つかりませんでした");
		return false;
	}
	// インデックスバッファをバインド
	uint32_t indexCount = m_IndexBufferManager.BindIndexData(_ibName, DirectX11::Get::GetContext());
	if (indexCount == UINT_FAST32_MAX) {
		ErrorLog::OutputToConsole("インデックスバッファが見つかりませんでした");
		return false;
	}

	// シェーダーの定数バッファ情報をバインド
	m_CBManager.BindConstantBuffer(vsCB, DirectX11::Get::GetContext(), VERTEXSHADER);
	m_CBManager.BindConstantBuffer(psCB, DirectX11::Get::GetContext(), PIXSELSHADER);

	// テクスチャバインド
	if (_textureName != nullptr)
	{
		// テクスチャ・サンプラー バインド
		m_ViewManager.BindSRV(_textureName, DirectX11::Get::GetContext(), PIXSELSHADER);
		m_SamplerManager.BindSampler(_sampler, DirectX11::Get::GetContext());
	}

	// 描画
	DirectX11::Get::GetContext()->DrawIndexed(indexCount, 0, 0);

	return true;
}


// ===================================================
// モデルを描画
// ===================================================
bool DirectX_DrawManager::DrawModelObject(
	const char* _vsShaderName,
	const char* _psShaderName,
	const char* _modelName)
{
	// シェーダーバインド
	const std::vector<ConstantBufferInfo>* vsCB = m_ShaderManager.BindVertexShader(_vsShaderName, DirectX11::Get::GetContext());
	const std::vector<ConstantBufferInfo>* psCB = m_ShaderManager.BindPixelShader(_psShaderName, DirectX11::Get::GetContext());

	// 頂点シェーダーの定数バッファ情報をバインド
	m_CBManager.BindConstantBuffer(vsCB, DirectX11::Get::GetContext(), VERTEXSHADER);

	// モデル情報を取得
	const ModelManagerData* data = m_ModelManager.GetModelData(_modelName);
	const std::string materialCBName = m_ModelManager.GetMaterialCBName();

	for (int i = 0; i < data->meshMaterialIDs.size(); i++)
	{
		// 頂点バッファをバインド
		m_VBManager.BindVertexBuffer(_modelName + std::to_string(i), DirectX11::Get::GetContext());

		// インデックスバッファをバインド
		uint32_t count = m_IndexBufferManager.BindIndexData(_modelName + std::to_string(i), DirectX11::Get::GetContext());

		// マテリアル情報取得
		Color color[3] = {
			data->materialData[data->meshMaterialIDs[i]].diffuse,
			data->materialData[data->meshMaterialIDs[i]].ambient,
			data->materialData[data->meshMaterialIDs[i]].specular
		};

		// マテリアル用定数バッファを更新
		UpdateShaderConstants(materialCBName.c_str(),
			&color,
			sizeof(Color) * 3);

		// ピクセルシェーダー更新
		m_CBManager.BindConstantBuffer(psCB, DirectX11::Get::GetContext(), PIXSELSHADER);

		// テクスチャバインド
		if (!data->materialData[data->meshMaterialIDs[i]].textureName.empty())
		{
			// テクスチャ バインド
			m_ViewManager.BindSRV(data->materialData[data->meshMaterialIDs[i]].textureName,
				DirectX11::Get::GetContext(),
				PIXSELSHADER);
			// サンプラー バインド
			m_SamplerManager.BindSampler(SamplerDesc::NormalSampler(), DirectX11::Get::GetContext());
		}

		// 描画
		DirectX11::Get::GetContext()->DrawIndexed(count, 0, 0);
	}

	return true;
}
