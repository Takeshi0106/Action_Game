
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
	DirectX11_ResourceReference _managers)
	: BaseDrawCreate(_windowWidth, _windowHeight),
	m_Device(_device),
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
	const PrimitiveType _type,
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

	return Handle();
}


// =======================================
// テクスチャのロード
// =======================================
Handle DirectX11_DrawCreate::LoadTexture(
	const Hashed_String& _textureName, 
	const String& _textureFolderName)
{
	return Handle();
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
