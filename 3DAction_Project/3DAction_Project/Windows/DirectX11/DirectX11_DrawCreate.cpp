
// =======================================
// ヘッダー
// =======================================
// 必須ヘッダー
#include "DirectX11_DrawCreate.h"
// DirectX11ヘッダー
#include <d3d11.h>
// DirectX11リソース管理ヘッダー
#include "DirectX11_ShaderManager.h"
#include "DirectX11_VertexBufferManager.h"
#include "DirectX11_IndexBufferManager.h"
#include "DirectX11_ConstantBufferManager.h"
#include "DirectX11_TextureHandleManager.h"
// モデル管理ヘッダー
#include "../../MeshMaterialManager.h"
// 変換ヘッダー
#include "DirectX11_FormatConverter.h"
// ファイルシステムヘッダー
#include <filesystem>


// =======================================
// コンストラクタ
// =======================================
DirectX11_DrawCreate::DirectX11_DrawCreate(
	uint32_t _windowWidth,
	uint32_t _windowHeight,
	ID3D11Device* _device,
	ID3D11DeviceContext* _deviceContext,
	DirectX11_ResourceReference _managers)
	: BaseDrawCreate(_windowWidth, _windowHeight),
	m_Device(_device),
	m_DeviceContext(_deviceContext),
	m_Managers(_managers)
{

}


// =======================================
// 頂点バッファ作成
// =======================================
Handle DirectX11_DrawCreate::CreateVertexBuffer(
	const Hashed_String& _vbName,
	const BinaryView& _data,
	const uint32_t _vertexNumber,
	const BufferUsage _usage,
	const CPUAccess _access)
{
	// DirectX11用のバッファ記述作成
	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.ByteWidth = static_cast<UINT>(_data.GetSize());
	bufferDesc.Usage = DirectX11_FormatConverter::ToDXUsage(_usage);
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = DirectX11_FormatConverter::ToDXCPUAccess(_access);

	// 初期データ設定
	D3D11_SUBRESOURCE_DATA initData{};
	initData.pSysMem = _data.GetData();

	// ストライド計算
	const uint32_t stride = static_cast<uint32_t>(_data.GetSize() / _vertexNumber);

	// マネージャー登録
	return m_Managers.vertexBufferManager.VertexBufferCreateOnGet(
		m_Device,
		_vbName,
		bufferDesc,
		&initData,
		_vertexNumber,
		stride);
}


// =======================================
// インデックスバッファ作成
// =======================================
Handle DirectX11_DrawCreate::CreateIndexBuffer(
	const Hashed_String& _indexName,
	const uint32_t* _indexData,
	const size_t _indexSize,
	const uint32_t _indexNumber, 
	const BufferUsage _usage,
	const CPUAccess _access)
{
	// DirectX11用のバッファ記述作成
	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.ByteWidth = static_cast<UINT>(_indexSize);
	bufferDesc.Usage = DirectX11_FormatConverter::ToDXUsage(_usage);
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = DirectX11_FormatConverter::ToDXCPUAccess(_access);

	// 初期データ設定
	D3D11_SUBRESOURCE_DATA initData{};
	initData.pSysMem = _indexData;

	// フォーマット設定
	const DXGI_FORMAT format = DXGI_FORMAT_R32_UINT;

	// マネージャー登録
	return m_Managers.indexBufferManager.IndexBufferCreateOnGet(
		m_Device,
		_indexName,
		bufferDesc,
		&initData,
		_indexNumber,
		format);
}


// =======================================
// 定数バッファ作成
// =======================================
Handle DirectX11_DrawCreate::CreateConstantBuffer(
	const Hashed_String& _constantName,
	const BinaryView& _data,
	const BufferUsage _usage,
	const CPUAccess _access)
{
	// DirectX11用のバッファ記述作成
	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.ByteWidth = static_cast<UINT>(_data.GetSize());
	bufferDesc.Usage = DirectX11_FormatConverter::ToDXUsage(_usage);
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = DirectX11_FormatConverter::ToDXCPUAccess(_access);

	// 初期データ設定
	D3D11_SUBRESOURCE_DATA initData{};
	initData.pSysMem = _data.GetData();

	// マネージャー登録
	return m_Managers.constantBufferManager.ConstantBufferCreateOnGet(
		m_Device,
		_constantName,
		bufferDesc,
		&initData);
}


// =======================================
// テクスチャ作成
// =======================================
Handle DirectX11_DrawCreate::CreateTexture(
	const Hashed_String& _name,
	const TextureCreateDesc& _desc)
{
#if defined(DEBUG) || defined(_DEBUG)
	if (_desc.width == 0 || _desc.height == 0)
	{
		ErrorLog::OutputToConsole(
			u8"テクスチャ作成サイズに0が使用されています");
	}
#endif

	// サイズ
	uint32_t width = 0;
	uint32_t height = 0;

	// テクスチャデスク
	D3D11_TEXTURE2D_DESC textureDesc{};
	// サンプラーデスク
	D3D11_SAMPLER_DESC sampDesc{};
	// SRVデスク
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDescTemp{};
	// SRV作成デスクポインタ
	D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc = nullptr;
	// RTVデスク
	D3D11_RENDER_TARGET_VIEW_DESC rtvDescTemp{};
	// RTV作成デスクポインタ
	D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc = nullptr;
	// DSVデスク
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDescTemp{};
	// DSV作成デスクポインタ
	D3D11_DEPTH_STENCIL_VIEW_DESC* dsvDesc = nullptr;

	// サイズ計算
	if (_desc.sizeType == TextureSizeType::Relative)
	{
		// 相対サイズの場合、ウィンドウサイズから計算
		width = static_cast<uint32_t>(_desc.width * m_ScreenWidth);
		height = static_cast<uint32_t>(_desc.height * m_ScreenHeight);
	}
	else if(_desc.sizeType == TextureSizeType::Absolute)
	{
		// 絶対サイズの場合、そのまま使用
		width = static_cast<uint32_t>(_desc.width);
		height = static_cast<uint32_t>(_desc.height);
	}

	// テクスチャデスク作成
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DirectX11_FormatConverter::ToDXFormat(_desc.format);
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = DirectX11_FormatConverter::ToDXUsage(_desc.usage);
	textureDesc.BindFlags = DirectX11_FormatConverter::ConvertBindFlag(_desc.bindFlags);
	textureDesc.CPUAccessFlags = DirectX11_FormatConverter::ToDXCPUAccess(_desc.cpuAccess);
	textureDesc.MiscFlags = 0;

	// ミップ処理切り替え
	switch (_desc.mipMapType)
	{
		case MipMapType::None:
			textureDesc.MipLevels = 1;
			break;

		case MipMapType::Auto:
#if defined(DEBUG) || defined(_DEBUG)
			if (!(_desc.bindFlags & BindFlag::Bind_ShaderResource) || !(_desc.bindFlags & BindFlag::Bind_RenderTarget))
			{
				ErrorLog::OutputToConsole(u8"自動ミップ生成したいならSRV と RTV の両方が必要");
			}
#endif
			textureDesc.MiscFlags |= D3D11_RESOURCE_MISC_GENERATE_MIPS;
			// ミップマップレベルをフルに設定
			textureDesc.MipLevels = 0;
			break;

		case MipMapType::Manual:
			// 手動設定の場合、ミップマップレベルを指定
			textureDesc.MipLevels = _desc.mipLevels;
			break;
	}

	// サンプラーデスク作成
	sampDesc.Filter = DirectX11_FormatConverter::ConvertFilter(_desc.sampler.filter);
	sampDesc.AddressU = DirectX11_FormatConverter::ConvertAddressMode(_desc.sampler.addressU);
	sampDesc.AddressV = DirectX11_FormatConverter::ConvertAddressMode(_desc.sampler.addressV);
	sampDesc.AddressW = DirectX11_FormatConverter::ConvertAddressMode(_desc.sampler.addressW);
	sampDesc.MipLODBias = 0.0f;
	sampDesc.MaxAnisotropy = (_desc.sampler.filter == SamplerFilter::Anisotropic) ? 16 : 1;
	sampDesc.ComparisonFunc = DirectX11_FormatConverter::ConvertComparisonFunc(_desc.sampler.comparisonFunc);
	sampDesc.BorderColor[0] = 0.0f;
	sampDesc.BorderColor[1] = 0.0f;
	sampDesc.BorderColor[2] = 0.0f;
	sampDesc.BorderColor[3] = 0.0f;
	sampDesc.MinLOD = 0.0f;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// SRVデスク作成
	if (_desc.bindFlags & BindFlag::Bind_ShaderResource)
	{
		srvDescTemp.Format = textureDesc.Format;
		srvDescTemp.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDescTemp.Texture2D.MostDetailedMip = 0;
		srvDescTemp.Texture2D.MipLevels =
			(textureDesc.MipLevels == 0) ? -1 : textureDesc.MipLevels;
		// ポインターに代入
		srvDesc = &srvDescTemp;
	}
	// RTVデスク作成
	if (_desc.bindFlags & BindFlag::Bind_RenderTarget)
	{
		rtvDescTemp.Format = textureDesc.Format;
		rtvDescTemp.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		rtvDescTemp.Texture2D.MipSlice = 0;
		// ポインターに代入
		rtvDesc = &rtvDescTemp;
	}
	// DSVデスク作成
	if (_desc.bindFlags & BindFlag::Bind_DepthStencil)
	{
		dsvDescTemp.Format = textureDesc.Format;
		dsvDescTemp.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		dsvDescTemp.Texture2D.MipSlice = 0;
		// ポインターに代入
		dsvDesc = &dsvDescTemp;
	}

	// マネージャー登録
	Handle handle = m_Managers.textureManager.CreateTextures(
		m_Device,
		_name,
		textureDesc,
		sampDesc,
		_desc.sampler,
		srvDesc,
		rtvDesc,
		dsvDesc);

	// ミップマップ自動生成
	if (_desc.mipMapType == MipMapType::Auto && handle.IsValid())
	{
		// テクスチャ関連情報取得
		DirectX11_TextureStruct texture = m_Managers.textureManager.GetTextureHandle(handle);
		// デバイスコンテキストを使ってミップマップ自動生成
		m_DeviceContext->GenerateMips(texture.srvData->GetSRV());
	}

	return handle;
}


// =======================================
// テクスチャのロード
// =======================================
Handle DirectX11_DrawCreate::LoadTexture(
	const Hashed_String& _textureName, 
	const TextureLoadDesc& _loadDesc,
	const String& _textureFolderName)
{
	D3D11_SAMPLER_DESC samp{};

	// サンプラーデスク作成
	samp.Filter = DirectX11_FormatConverter::ConvertFilter(_loadDesc.sampler.filter);
	samp.AddressU = DirectX11_FormatConverter::ConvertAddressMode(_loadDesc.sampler.addressU);
	samp.AddressV = DirectX11_FormatConverter::ConvertAddressMode(_loadDesc.sampler.addressV);
	samp.AddressW = DirectX11_FormatConverter::ConvertAddressMode(_loadDesc.sampler.addressW);
	samp.MipLODBias = 0.0f;
	samp.MaxAnisotropy = (_loadDesc.sampler.filter == SamplerFilter::Anisotropic) ? 16 : 1;
	samp.ComparisonFunc = DirectX11_FormatConverter::ConvertComparisonFunc(_loadDesc.sampler.comparisonFunc);
	samp.BorderColor[0] = 0.0f;
	samp.BorderColor[1] = 0.0f;
	samp.BorderColor[2] = 0.0f;
	samp.BorderColor[3] = 0.0f;
	samp.MinLOD = 0.0f;
	samp.MaxLOD = D3D11_FLOAT32_MAX;

	// ファイルからテクスチャをロード
	Handle handle = m_Managers.textureManager.LoadFaileTexture_TextureFolder(
		m_Device,
		m_DeviceContext,
		_textureName,
		_loadDesc,
		samp,
		_textureFolderName);

	return handle;
}


// =======================================
// モデルのロード
// =======================================
Handle DirectX11_DrawCreate::LoadModel(
	const Hashed_String& _modelName, 
	const String& _modelFolderName)
{
	return Handle();
}
