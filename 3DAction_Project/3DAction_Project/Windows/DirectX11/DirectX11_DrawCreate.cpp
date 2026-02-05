
// =======================================
// ヘッダー
// =======================================
// 必須ヘッダー
#include "DirectX11_DrawCreate.h"
// DirectX11リソース管理ヘッダー
#include "DirectX11_ShaderManager.h"
#include "DirectX11_VertexBufferManager.h"
#include "DirectX11_IndexBufferManager.h"
#include "DirectX11_ConstantBufferManager.h"
#include "DirectX11_Texture2DBufferManager.h"
#include "DirectX11_SamplerManager.h"
#include "DirectX11_ViewManager.h"
// モデル管理ヘッダー
#include "../../MeshMaterialManager.h"
// 変換ヘッダー
#include "DirectX11_FormatConverter.h"
// ファイルシステムヘッダー
#include <filesystem>


// =======================================
// 頂点バッファ作成
// =======================================
const Handle DirectX11_DrawCreate::CreateVertexBuffer(
	const Hashed_String& _vbName,
	const void* _data,
	const size_t _size,
	const uint32_t _vertexNumber,
	const PrimitiveType _type,
	const BufferUsage _usage,
	const CPUAccess _access)
{
	// マネージャー登録
	return m_VertexBufferManager.VertexBufferCreateOnGet(
		m_Device,
		_data,
		_size,
		_vertexNumber,
		DirectX11_FormatConverter::ToDXUsage(_usage),
		(D3D11_CPU_ACCESS_FLAG)DirectX11_FormatConverter::ToDXCPUAccess(_access),
		_vbName);
}


// =======================================
// インデックスバッファ作成
// =======================================
const Handle DirectX11_DrawCreate::CreateIndexBuffer(
	const Hashed_String& _indexName,
	const uint32_t* _indexData,
	const size_t _indexSize,
	const uint32_t _indexNumber, 
	const BufferUsage _usage,
	const CPUAccess _access)
{
	// マネージャー登録
	return m_IndexBufferManager.IndexBufferCreateOnGet(
		m_Device,
		_indexData,
		_indexSize,
		_indexNumber,
		DirectX11_FormatConverter::ToDXUsage(_usage),
		(D3D11_CPU_ACCESS_FLAG)DirectX11_FormatConverter::ToDXCPUAccess(_access),
		_indexName);
}


// =======================================
// 定数バッファ作成
// =======================================
const Handle DirectX11_DrawCreate::CreateConstantBuffer(
	const Hashed_String& _constantName,
	const size_t _size,
	const void* _data,
	const BufferUsage _usage,
	const CPUAccess _access)
{
	// マネージャー登録
	return m_ConstantBufferManager.ConstantBufferCreateOnGet(
		m_Device,
		_size,
		DirectX11_FormatConverter::ToDXUsage(_usage),
		(D3D11_CPU_ACCESS_FLAG)DirectX11_FormatConverter::ToDXCPUAccess(_access),
		_constantName,
		_data);
}

// =======================================
// テクスチャ作成
// =======================================
const Handle DirectX11_DrawCreate::CreateTexture(
	const Hashed_String& _name,
	const uint16_t _width,
	const uint16_t _height,
	const Format _format,
	const BindFlag _bindFlag,
	const BufferUsage _usage,
	const CPUAccess _cpu)
{
	// デスク作成
	D3D11_TEXTURE2D_DESC desc{};
	desc.Width = _width;
	desc.Height = _height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DirectX11_FormatConverter::ToDXFormat(_format);
	desc.SampleDesc.Count = 1;
	desc.Usage = DirectX11_FormatConverter::ToDXUsage(_usage);
	desc.BindFlags = static_cast<UINT>(_bindFlag);
	desc.CPUAccessFlags = DirectX11_FormatConverter::ToDXCPUAccess(_cpu);

	// マネージャー登録
	return m_Texture2DBufferManager.Texture2DBufferCreateOnGet(
		m_Device,
		&desc,
		_name);
}


// =======================================
// サンプラー作成
// =======================================
const Handle DirectX11_DrawCreate::CreateSampler(
	const Hashed_String& _samplerName, 
	const SamplerDesc& _desc)
{
	// デスク作成
	D3D11_SAMPLER_DESC desc{};
	desc.Filter = DirectX11_FormatConverter::ConvertFilter(_desc.filter);
	desc.AddressU = DirectX11_FormatConverter::ConvertAddressMode(_desc.addressU);
	desc.AddressV = DirectX11_FormatConverter::ConvertAddressMode(_desc.addressV);
	desc.AddressW = DirectX11_FormatConverter::ConvertAddressMode(_desc.addressW);
	desc.ComparisonFunc = DirectX11_FormatConverter::ConvertComparisonFunc(_desc.comparisonFunc);
	desc.MinLOD = 0;
	desc.MaxLOD = D3D11_FLOAT32_MAX;

	// マネージャー登録
	return m_SamplerManager.SamplerStateCreateOnGet(
		m_Device,
		desc,
		_samplerName);
}

// =======================================
// View作成
// =======================================
// SRV作成
void DirectX11_DrawCreate::CreateSRV(
	const Hashed_String& _name,
	const Format _format, 
	TextureHandle& _outTextureHandle,
	const uint16_t _mostDetailedMip, 
	const int16_t _mipLevels)
{
	// テクスチャ取得
	ID3D11Texture2D* tex = m_Texture2DBufferManager.GetTexture2DBuffer(_outTextureHandle.textureHandle);

	// SRV作成
	_outTextureHandle.srvHandle = m_ViewManager.ShaderResourceViewCreateOnGet(
		m_Device,
		tex,
		_mostDetailedMip,
		_mipLevels,
		_name);
}

// RTV作成
void DirectX11_DrawCreate::CreateRTV(
	const Hashed_String& name, 
	const uint16_t mipSlice,
	TextureHandle& _textureHandle)
{
	// テクスチャ取得
	ID3D11Texture2D* tex = m_Texture2DBufferManager.GetTexture2DBuffer(_textureHandle.textureHandle);

	// RTV作成
	_textureHandle.rtvHandle = m_ViewManager.RenderTargetViewCreateOnGet(
		m_Device,
		tex,
		mipSlice,
		name);
}

// DSV作成
void DirectX11_DrawCreate::CreateDSV(
	const Hashed_String& name, 
	const Format format,
	TextureHandle& _textureHandle)
{
	// テクスチャ取得
	ID3D11Texture2D* tex = m_Texture2DBufferManager.GetTexture2DBuffer(_textureHandle.textureHandle);

	// DSV作成
	_textureHandle.dsvHandle = m_ViewManager.DepthStencilViewCreateOnGet(
		m_Device,
		tex,
		DirectX11_FormatConverter::ToDXFormat(format),
		name);
}


// =======================================
// テクスチャのロード
// =======================================
void DirectX11_DrawCreate::LoadTexture(
	const Hashed_String& _textureName, 
	TextureHandle& _outTextureHandle,
	const int16_t _mipLevel, 
	const String& _textureFolderName)
{
	//return m_TextureLoad.LoadFaileTexture_TextureFolder(
	//	m_Device,
	//	_textureName,
	//	_textureFolderName,
	//	(uint32_t)_mipLevel,
	//	m_Texture2DBufferManager,
	//	m_ViewManager);
}


// =======================================
// モデルのロード
// =======================================
const Handle DirectX11_DrawCreate::LoadModel(
	const Hashed_String& _modelName, 
	const String& _modelFolderName)
{
	// すでにモデルが存在する場合はハンドルを返す
	if (m_ModelManager.CheckModelHandle(_modelName)) {
		return m_ModelManager.GetModelHandle(_modelName);
	}

	// モデルデータを取得
	ModelData modelData = m_ModelLoad.ModelLoad(_modelName.GetString(), _modelFolderName);

	// テクスチャパス取得
	std::filesystem::path modelPath = m_ModelLoad.GetModelLoadPath().GetU8String();

	if (!_modelFolderName.GetU8String().empty()) {
		modelPath /= _modelFolderName.GetU8String();
	}

	// モデルハンドル
	ModelHandle modelHandle{};
	modelHandle.meshHandles.resize(modelData.meshDataArray.size());
	
	// マテリアルハンドル配列
	std::vector<TextureHandle> textureHandles;
	std::vector<Handle> materialHandles;

	// 配列リサイズ
	textureHandles.resize(modelData.materialDataArray.size());
	materialHandles.resize(modelData.materialDataArray.size());

	// --------------------------------
	// マテリアル情報作成
	// --------------------------------
	for (int i = 0; i < modelData.materialDataArray.size(); i++)
	{
		// マテリアル名作成
		String name = _modelName.GetString().GetU8String() + u8"_Material_" + String::to_u8string(i);

		// メッシュマテリアル情報取得
		MeshMaterialData& materialData = modelData.materialDataArray[i];

		TextureHandle textureHandle = {};
		std::filesystem::path texturePath = modelPath / materialData.textureName.GetString().GetU8String();

		if (!materialData.textureName.GetString().GetU8String().empty())
		{
			//// テクスチャロード
			//textureHandle = m_TextureLoad.LoadFaileTexture(
			//	m_Device,
			//	texturePath.u8string(),
			//	1,
			//	m_Texture2DBufferManager,
			//	m_ViewManager);
		}

		// マテリアル取得
		MeshMaterialColor materialColors = {
			materialData.diffuse,
			materialData.specular,
			materialData.ambient };

		// マテリアル登録
		Handle materialHandle = m_MaterialManager.AddMeshMaterialData(name, materialColors);

		// ハンドルに登録
		materialHandles.push_back(materialHandle);
		textureHandles.push_back(textureHandle);
	}


	// --------------------------------
	// バッファ作成
	// --------------------------------
	for (int i = 0; i < modelData.meshDataArray.size(); i++)
	{
		// メッシュデータ取得
		MeshData& mesh = modelData.meshDataArray[i];

		// 登録名作成
		String meshName = _modelName.GetString().GetU8String() + String::to_u8string(i);

		//// 頂点バッファ作成
		//Handle vbHandle = CreateVertexBuffer(
		//	meshName,
		//	mesh.vertices.data(),
		//	sizeof(Vertex) * mesh.vertices.size(),
		//	static_cast<uint32_t>(mesh.vertices.size()),
		//	PrimitiveType::TriangleList,
		//	BufferUsage::Default,
		//	CPUAccess::None);

		//// インデックスバッファ作成
		//Handle ibHandle = CreateIndexBuffer(
		//	meshName,
		//	mesh.indices.data(),
		//	mesh.indices.size() * sizeof(uint32_t),
		//	static_cast<uint32_t>(mesh.indices.size()),
		//	BufferUsage::Default,
		//	CPUAccess::None);

		// ハンドルに登録
		//modelHandle.meshHandles[i].meshVertexBufferHandles = vbHandle;
		//modelHandle.meshHandles[i].meshIndexBufferHandles = ibHandle;
		// マテリアル・テクスチャハンドル登録
		modelHandle.meshHandles[i].materialHandle = materialHandles[mesh.materialID];
		modelHandle.meshHandles[i].textureHandle = textureHandles[mesh.materialID];
	}

	// モデルハンドルを返す
	return m_ModelManager.AddModelHandle(_modelName, modelHandle);
}
