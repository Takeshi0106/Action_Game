
// =======================================
// ヘッダー
// =======================================
// 必須ヘッダー
#include "DirectX11_DrawCreate.h"
// 変換ヘッダー
#include "DirectX11/DirectX11_FormatConverter.h"


// =======================================
// 頂点バッファ作成
// =======================================
const Handle DirectX11_DrawCreate::CreateVertexBuffer(
	const char* _vbName,
	const void* _data,
	const size_t _size,
	const uint32_t _vertexNumber,
	const PrimitiveType _type,
	const BufferUsage _usage,
	const CPUAccess _access)
{
	// すでに存在する場合はハンドルを返す
	if (m_VertexBufferManager.Exists(_vbName)) {
		return m_VertexBufferManager.GetVertexBufferHandle(_vbName);
	}

	// マネージャー登録
	return m_VertexBufferManager.VertexBufferCreate(
		m_Device,
		_data,
		_size,
		DirectX11_FormatConverter::ToDXUsage(_usage),
		(D3D11_CPU_ACCESS_FLAG)DirectX11_FormatConverter::ToDXCPUAccess(_access),
		_vbName);
}


// =======================================
// インデックスバッファ作成
// =======================================
const Handle DirectX11_DrawCreate::CreateIndexBuffer(
	const char* _indexName,
	const uint32_t* _indexData,
	const uint32_t _indexNumber, 
	const BufferUsage _usage,
	const CPUAccess _access)
{
	// すでに存在する場合はハンドルを返す
	if (m_ConstantBufferManager.Exists(_indexName)) {
		return m_IndexBufferManager.GetIndexBufferHandle(_indexName);
	}

	// マネージャー登録
	return m_IndexBufferManager.IndexBufferCreate(
		m_Device,
		_indexData,
		_indexNumber,
		DirectX11_FormatConverter::ToDXUsage(_usage),
		(D3D11_CPU_ACCESS_FLAG)DirectX11_FormatConverter::ToDXCPUAccess(_access),
		_indexName);
}


// =======================================
// 定数バッファ作成
// =======================================
const Handle DirectX11_DrawCreate::CreateConstantBuffer(
	const char* _constantName,
	const size_t _size,
	const void* _data,
	const BufferUsage _usage,
	const CPUAccess _access)
{
	// すでに存在する場合はハンドルを返す
	if (m_ConstantBufferManager.Exists(_constantName))
	{
		return m_ConstantBufferManager.GetConstantBufferHandle(_constantName);
	}

	// マネージャー登録
	return m_ConstantBufferManager.ConstantBufferCreate(
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
	const char* _name,
	const uint32_t _width,
	const uint32_t _height,
	const Format _format,
	const BindFlag _bindFlag,
	const BufferUsage _usage,
	const CPUAccess _cpu)
{
	// すでに存在する場合はハンドルを返す
	if (m_Texture2DBufferManager.Exists(_name))
	{
		return m_Texture2DBufferManager.GetHandle(_name);
	}

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
	return m_Texture2DBufferManager.Texture2DBufferCreate(
		m_Device,
		&desc,
		_name);
}


// =======================================
// サンプラー作成
// =======================================
const Handle DirectX11_DrawCreate::CreateSampler(const char* _samplerName, const SamplerDesc& _desc)
{
	// すでに存在する場合はハンドルを返す
	if (m_SamplerManager.Exists(_samplerName))
	{
		return m_SamplerManager.GetHandle(_samplerName);
	}

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
	return m_SamplerManager.SamplerStateCreate(
		m_Device,
		desc,
		_samplerName);
}

// =======================================
// View作成
// =======================================
// SRV作成
const Handle DirectX11_DrawCreate::CreateSRV(const Handle& _textureHandle, const char* _name,
	const Format _format, const uint32_t _mostDetailedMip, const int32_t _mipLevels)
{
	// すでに存在する場合はハンドルを返す
	if (m_ViewManager.ExistsSRV(_name))
	{
		return m_ViewManager.GetSRVHandle(_name);
	}

	// テクスチャ取得
	ID3D11Texture2D* tex = m_Texture2DBufferManager.GetTexture2DBuffer(_textureHandle);

	// SRV作成
	return m_ViewManager.ShaderResourceViewCreate(
		m_Device,
		tex,
		_mostDetailedMip,
		_mipLevels,
		_name);
}

// RTV作成
const Handle DirectX11_DrawCreate::CreateRTV(const Handle& _textureHandle, const char* name, const uint32_t mipSlice)
{
	// すでに存在する場合はハンドルを返す
	if (m_ViewManager.ExistsRTV(name))
	{
		return m_ViewManager.GetRTVHandle(name);
	}

	// テクスチャ取得
	ID3D11Texture2D* tex = m_Texture2DBufferManager.GetTexture2DBuffer(_textureHandle);

	// RTV作成
	return m_ViewManager.RenderTargetViewCreate(
		m_Device,
		tex,
		mipSlice,
		name);
}

// DSV作成
const Handle DirectX11_DrawCreate::CreateDSV(const Handle& _textureHandle, const char* name, const Format format)
{
	// すでに存在する場合はハンドルを返す
	if (m_ViewManager.ExistsDSV(name)) {
		return m_ViewManager.GetDSVHandle(name);
	}

	// テクスチャ取得
	ID3D11Texture2D* tex = m_Texture2DBufferManager.GetTexture2DBuffer(_textureHandle);

	// DSV作成
	return m_ViewManager.DepthStencilViewCreate(
		m_Device,
		tex,
		DirectX11_FormatConverter::ToDXFormat(format),
		name);
}


// =======================================
// テクスチャのロード
// =======================================
const TextureHandle DirectX11_DrawCreate::LoadTexture(const char* textureName, const char* textureFolderName)
{

	return TextureHandle{};
}


// =======================================
// モデルのロード
// =======================================
const Handle DirectX11_DrawCreate::LoadModel(const char* modelName, const char* modelFolderName)
{
	// すでにモデルが存在する場合はハンドルを返す
	if (m_ModelManager.CheckModelHandle(modelName)) {
		return m_ModelManager.GetModelHandle(modelName);
	}

	// モデルデータを取得
	ModelData modelData = m_ModelLoad.ModelLoad(modelName, modelFolderName);

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
		std::string name = std::string(modelName) + "_Material_" + std::to_string(i);

		// メッシュマテリアル情報取得
		MeshMaterialData& materialData = modelData.materialDataArray[i];

		TextureHandle textureHandle = {};

		if (!materialData.textureName.empty())
		{
			// テクスチャロード
			textureHandle = LoadTexture(materialData.textureName.c_str(), modelFolderName);
		}

		// マテリアル取得
		MeshMaterialColor materialColors = {
			materialData.diffuse,
			materialData.specular,
			materialData.ambient };

		// マテリアル登録
		Handle materialHandle = m_MaterialManager.AddMeshMaterialData(name.c_str(), materialColors);

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
		std::string meshName = std::string(modelName) + std::to_string(i);

		// 頂点バッファ作成
		Handle vbHandle = CreateVertexBuffer(
			meshName.c_str(),
			mesh.vertices.data(),
			sizeof(Vertex) * mesh.vertices.size(),
			static_cast<uint32_t>(mesh.vertices.size()),
			PrimitiveType::TriangleList,
			BufferUsage::Default,
			CPUAccess::None);

		// インデックスバッファ作成
		Handle ibHandle = CreateIndexBuffer(
			meshName.c_str(),
			mesh.indices.data(),
			static_cast<uint32_t>(mesh.indices.size()),
			BufferUsage::Default,
			CPUAccess::None);

		// ハンドルに登録
		modelHandle.meshHandles[i].meshVertexBufferHandles = vbHandle;
		modelHandle.meshHandles[i].meshIndexBufferHandles = ibHandle;
		// マテリアル・テクスチャハンドル登録
		modelHandle.meshHandles[i].materialHandle = materialHandles[mesh.materialID];
		modelHandle.meshHandles[i].textureHandle = textureHandles[mesh.materialID];
	}

	// モデルハンドルを返す
	return m_ModelManager.AddModelHandle(modelName, modelHandle);
}
