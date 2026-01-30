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
// DirectX11リソース管理ヘッダー
#include "DirectX11_ShaderManager.h"
#include "DirectX11_VertexBufferManager.h"
#include "DirectX11_IndexBufferManager.h"
#include "DirectX11_ConstantBufferManager.h"
#include "DirectX11_Texture2DBufferManager.h"
#include "DirectX11_SamplerManager.h"
#include "DirectX11_ViewManager.h"
// 共通リソース管理ヘッダー
#include "../ModelLoadManager.h"
#include "../MeshMaterialManager.h"
// モジュール
#include "DirectX11_TextureLoadModule.h"
#include "../ModelLoadeModule.h"
// 基底ヘッダー
#include "../BaseDrawCreate.h"


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

	// リソースマネージャーの参照
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
	const Handle CreateVertexBuffer(
		const String& _vbName,
		const void* _data,
		const size_t _size,
		const uint32_t _vertexNumber,
		const PrimitiveType _type = PrimitiveType::TriangleStrip,
		const BufferUsage _usage = BufferUsage::Dynamic,
		const CPUAccess _access = CPUAccess::Write) override;

	// インデックスバッファ作成
	const Handle CreateIndexBuffer(
		const String& _indexName,
		const uint32_t* _indexData,
		const size_t _indexSize,
		const uint32_t _indexNumber,
		const BufferUsage _usage,
		const CPUAccess _access) override;
	
	// 定数バッファ作成
	const Handle CreateConstantBuffer(
		const String& _constantName,
		const size_t _size,
		const void* _data = nullptr,
		const BufferUsage _usage = BufferUsage::Dynamic,
		const CPUAccess _access = CPUAccess::Write) override;

	// テクスチャ作成
	const Handle CreateTexture(
		const String& _name,
		const uint16_t _width,
		const uint16_t _height,
		const Format _format,
		const BindFlag _bindFlag,
		const BufferUsage _usage = BufferUsage::Default,
		const CPUAccess _cpu = CPUAccess::None) override;

	// サンプラー作成
	const Handle CreateSampler(const String& _samplerName, const SamplerDesc& _desc) override;

	// View作成
	const Handle CreateSRV(const Handle& _textureHandle, 
		const String& name,
		const Format format, 
		const uint16_t mostDetailedMip = 0, 
		const int16_t mipLevels = -1) override;
	const Handle CreateRTV(const Handle& _textureHandle, 
		const String& name, 
		const uint16_t mipSlice) override;
	const Handle CreateDSV(const Handle& _textureHandle, 
		const String& name, 
		const Format format) override;

	// テクスチャのロード
	const TextureHandle LoadTexture(const String& textureName, const int16_t _mipLevels = -1, const String& textureFolderName = u8"") override;
	// モデルのロード
	const Handle LoadModel(const Hashed_String& modelName, const String& modelFolderName = u8"") override;
};
