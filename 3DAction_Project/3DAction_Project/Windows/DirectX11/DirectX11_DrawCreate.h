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
// 共通リソース管理ヘッダー
#include "../../ModelLoadManager.h"
// モジュール
#include "DirectX11_TextureLoadModule.h"
#include "../../ModelLoadeModule.h"
// 基底ヘッダー
#include "../../BaseDrawCreate.h"
// キー文字列
#include "../../Hashed_String.h"


// ===============================================
// 前方宣言
// ===============================================
class DirectX11_ShaderManager;
class DirectX11_VertexBufferManager;
class DirectX11_IndexBufferManager;
class DirectX11_ConstantBufferManager;
class DirectX11_Texture2DBufferManager;
class DirectX11_SamplerManager;
class DirectX11_ViewManager;
class ModelLoadManager;
class MeshMaterialManager;


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
	// テクスチャ2Dバッファ
	DirectX11_Texture2DBufferManager& texture2DBufferManager;
	// サンプラー
	DirectX11_SamplerManager& samplerManager;
	// ビュー
	DirectX11_ViewManager& viewManager;
	// モデル
	ModelLoadManager& modelLoadManager;
	// マテリアル
	MeshMaterialManager& meshMaterialManager;
};


// ===============================================
// クラス
// ===============================================
class DirectX11_DrawCreate final : public BaseDrawCreate
{
private:
	// -------------------------------------------
	// メンバー変数
	// -------------------------------------------
	// コンテキスト
	ID3D11Device* m_Device;

	// 各リソースマネージャーの参照
	DirectX11_ShaderManager& m_ShaderManager;
	DirectX11_VertexBufferManager& m_VertexBufferManager;
	DirectX11_IndexBufferManager& m_IndexBufferManager;
	DirectX11_ConstantBufferManager& m_ConstantBufferManager;
	DirectX11_Texture2DBufferManager& m_Texture2DBufferManager;
	DirectX11_SamplerManager& m_SamplerManager;
	DirectX11_ViewManager& m_ViewManager;
	ModelLoadManager& m_ModelManager;
	MeshMaterialManager& m_MaterialManager;

	// モジュール
	DirectX11_TextureLoadModule m_TextureLoad;
	ModelLoadeModule m_ModelLoad;


public:
	// --------------------------------
	// コンストラクタ・デストラクタ
	// --------------------------------
	DirectX11_DrawCreate(
		ID3D11Device* _device,
		DirectX11_ResourceReference& _managers,
		const String& _texturePath,
		const String& _modelPath) :
		// デバイス
		m_Device(_device),
		// マネージャー参照
		m_ShaderManager(_managers.shaderManager),
		m_VertexBufferManager(_managers.vertexBufferManager),
		m_IndexBufferManager(_managers.indexBufferManager),
		m_ConstantBufferManager(_managers.constantBufferManager),
		m_Texture2DBufferManager(_managers.texture2DBufferManager),
		m_SamplerManager(_managers.samplerManager),
		m_ViewManager(_managers.viewManager),
		m_ModelManager(_managers.modelLoadManager),
		m_MaterialManager(_managers.meshMaterialManager),
		// パス
		m_TextureLoad(_texturePath),
		m_ModelLoad(_modelPath)
	{}

	~DirectX11_DrawCreate() override = default;


	// --------------------------------
	// リソース作成
	// --------------------------------
	// 頂点バッファ作成
	Handle CreateVertexBuffer(
		const Hashed_String& _vbName,
		const void* _data,
		const size_t _size,
		const uint32_t _vertexNumber,
		const PrimitiveType _type = PrimitiveType::TriangleStrip,
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
		const size_t _size,
		const void* _data = nullptr,
		const BufferUsage _usage = BufferUsage::Dynamic,
		const CPUAccess _access = CPUAccess::Write) override;

	// テクスチャ作成
	Handle CreateTexture(
		const Hashed_String& _name,
		const uint16_t _width,
		const uint16_t _height,
		const Format _format,
		const BindFlag _bindFlag,
		const BufferUsage _usage = BufferUsage::Default,
		const CPUAccess _cpu = CPUAccess::None) override;

	// サンプラー作成
	Handle CreateSampler(const Hashed_String& _name, const SamplerDesc& _desc) override;

	// View作成
	// SRV
	Handle CreateSRV(
		const Hashed_String& name,
		const Format format, 
		TextureHandle& _outTextureHandle,
		const uint16_t mostDetailedMip = 0, 
		const int16_t mipLevels = -1) override;
	// RTV
	Handle CreateRTV(
		const Hashed_String& name, 
		const uint16_t mipSlice,
		TextureHandle& _outTextureHandle) override;
	// DSV
	Handle CreateDSV(
		const Hashed_String& name, 
		const Format format,
		TextureHandle& _outTextureHandle) override;

	// テクスチャのロード
	Handle LoadTexture(
		const Hashed_String& textureName,
		TextureHandle& outTextureHandle,
		const int16_t _mipLevels = -1, 
		const String& textureFolderName = u8"") override;

	// モデルのロード
	Handle LoadModel(
		const Hashed_String& modelName, 
		const String& modelFolderName = u8"") override;
};
