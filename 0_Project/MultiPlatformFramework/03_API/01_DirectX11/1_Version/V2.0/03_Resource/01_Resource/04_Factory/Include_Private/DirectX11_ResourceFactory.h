#pragma once

// ===============================================
// クラス概要
// DirectX11用リソース作成クラス
// リソースの管理を行わず、作成のみを行う
// 
// 各リソースマネージャーとの作成を仲介するインターフェイスクラス
// ===============================================


// ===============================================
// ヘッダー
// ===============================================
// 基底ヘッダー
#include "IDrawCreate.h"
// キー文字列
#include "Hashed_String.h"
// モデルロードモジュール
#include "ModelLoadeModule.h"


// ===============================================
// 前方宣言
// ===============================================
// DirectX11デバイス
struct ID3D11Device;
struct ID3D11DeviceContext;

// DirectX11各リソースマネージャー
// シェーダー
class DirectX11_ShaderManager;
// 頂点バッファ
class DirectX11_VertexBufferManager;
// インデックスバッファ
class DirectX11_IndexBufferManager;
// 定数バッファ
class DirectX11_ConstantBufferManager;
// テクスチャ関連マネージャー
class DirectX11_TextureResourceManager;
// サンプラーマネージャー
class DirectX11_SamplerManager;
// マテリアルハンドルマネージャー
class MaterialHandleManager;
// モデルハンドルマネージャー
class ModelHandleManager;


// ===============================================
// リソースマネージャー構造体
// ===============================================
struct DirectX11_ResourceReference
{
	// シェーダー
	DirectX11_ShaderManager& shaderManager;
	// 頂点バッファ
	DirectX11_VertexBufferManager& vertexBufferManager;
	// インデックスバッファ
	DirectX11_IndexBufferManager& indexBufferManager;
	// 定数バッファ
	DirectX11_ConstantBufferManager& constantBufferManager;
	// テクスチャマネージャー
	DirectX11_TextureResourceManager& textureManager;
	// サンプラーマネージャー
	DirectX11_SamplerManager& samplerManager;
	// マテリアルハンドルマネージャー
	MaterialHandleManager& materialHandleManager;
	// モデルハンドルマネージャー
	ModelHandleManager& modelHandleManager;
};


// ===============================================
// クラス
// ===============================================
class DirectX11_ResourceFactory final : public IDrawCreate
{
private:
	// -------------------------------------------
	// メンバー変数
	// -------------------------------------------
	// コンテキスト
	ID3D11Device* m_Device;
	// デバイスコンテキスト
	ID3D11DeviceContext* m_DeviceContext;

	// 各リソースマネージャーの参照
	DirectX11_ResourceReference m_Managers;
	// モデルロードモジュール
	ModelLoadeModule m_ModelLoadModule;

	// 定数バッファ名
	Hashed_String m_MaterialCBName = Hashed_String(u8"Material");
	// マテリアル定数バッファハンドル
	Handle m_MaterialCBHandle = Handle();

public:
	// コンストラクタ・デストラクタ
	DirectX11_ResourceFactory(
		uint32_t _windowWidth,
		uint32_t _windowHeight,
		ID3D11Device* _device,
		ID3D11DeviceContext* _deviceContext,
		const DirectX11_ResourceReference _managers,
		const String& _modelFolderPath);
	~DirectX11_ResourceFactory() override = default;

	// 初期化
	bool Init();

	// --------------------------------
	// リソース作成
	// --------------------------------
	// 頂点バッファ作成
	Handle CreateVertexBuffer(
		const Hashed_String& _vbName,
		const BinaryView& _data,
		const uint32_t _vertexNumber,
		const BufferUsage _usage = BufferUsage::Dynamic,
		const CPUAccess _access = CPUAccess::Write) override;

	// インデックスバッファ作成
	Handle CreateIndexBuffer(
		const Hashed_String& _indexName,
		const uint32_t* _indexData,
		const size_t _indexSize,
		const uint32_t _indexNumber,
		const BufferUsage _usage,
		const CPUAccess _access) override;
	
	// 定数バッファ作成
	Handle CreateConstantBuffer(
		const Hashed_String& _constantName,
		const BinaryView& _data,
		const BufferUsage _usage = BufferUsage::Dynamic,
		const CPUAccess _access = CPUAccess::Write) override;

	// テクスチャ作成
	Handle CreateTexture(
		const Hashed_String& _textureName,
		const TextureCreateDesc& _textureDesc) override;

	// テクスチャのロード
	Handle LoadTexture(
		const Hashed_String& textureName,
		const TextureLoadDesc& _lodeDesc,
		const String& textureFolderName = u8"") override;

	// サンプラー作成
	Handle CreateSampler(
		const SamplerDesc& samplerDesc) override;

	// モデルのロード
	Handle LoadModel(
		const Hashed_String& modelName, 
		const Handle& _psShader,
		const Handle& _vsShader,
		const String& modelFolderName = u8"") override;
};
