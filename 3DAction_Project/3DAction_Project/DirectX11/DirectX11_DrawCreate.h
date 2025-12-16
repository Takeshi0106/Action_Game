#pragma once

// ===============================================
// クラス概要
// DirectX11用リソース作成クラス
// ===============================================


// ===============================================
// ヘッダー
// ===============================================
// リソース管理ヘッダー
#include "DirectX11_ShaderManager.h"
#include "DirectX11_VertexBufferManager.h"
#include "DirectX11_IndexBufferManager.h"
#include "DirectX11_ConstantBufferManager.h"
#include "DirectX11_Texture2DBufferManager.h"
#include "DirectX11_SamplerManager.h"
#include "DirectX11_ViewManager.h"
#include "../ModelLoadManager.h"
#include "../MeshMaterialManager.h"
// モジュール
#include "DirectX11_TextureLoadModule.h"
#include "../ModelLoadeModule.h"
// 基底ヘッダー
#include "../BaseDrawCreate.h"


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

	// リソース管理クラス
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
		DirectX11_ShaderManager& _shader,
		DirectX11_VertexBufferManager& _vertex,
		DirectX11_IndexBufferManager& _index,
		DirectX11_ConstantBufferManager& _constant,
		DirectX11_Texture2DBufferManager& _texture,
		DirectX11_SamplerManager& _sampler,
		DirectX11_ViewManager& _view,
		ModelLoadManager& _model,
		MeshMaterialManager& _material,
		const char* _texturePath,
		const char* _modelPath) :
		m_Device(_device),
		m_ShaderManager(_shader),
		m_VertexBufferManager(_vertex),
		m_IndexBufferManager(_index),
		m_ConstantBufferManager(_constant),
		m_Texture2DBufferManager(_texture),
		m_SamplerManager(_sampler),
		m_ViewManager(_view),
		m_ModelManager(_model),
		m_MaterialManager(_material),
		m_TextureLoad(_texturePath),
		m_ModelLoad(_modelPath)
	{}

	~DirectX11_DrawCreate() override = default;


	// --------------------------------
	// リソース作成
	// --------------------------------
	// 頂点バッファ作成
	const Handle CreateVertexBuffer(
		const char* _vbName,
		const void* _data,
		const size_t _size,
		const uint32_t _vertexNumber,
		const PrimitiveType _type = PrimitiveType::TriangleStrip,
		const BufferUsage _usage = BufferUsage::Dynamic,
		const CPUAccess _access = CPUAccess::Write) override;

	// インデックスバッファ作成
	const Handle CreateIndexBuffer(
		const char* _indexName,
		const uint32_t* _indexData,
		const uint32_t _indexNumber,
		const BufferUsage _usage,
		const CPUAccess _access) override;
	
	// 定数バッファ作成
	const Handle CreateConstantBuffer(
		const char* _constantName,
		const size_t _size,
		const void* _data = nullptr,
		const BufferUsage _usage = BufferUsage::Dynamic,
		const CPUAccess _access = CPUAccess::Write) override;

	// テクスチャ作成
	const Handle CreateTexture(
		const char* _name,
		const uint16_t _width,
		const uint16_t _height,
		const Format _format,
		const BindFlag _bindFlag,
		const BufferUsage _usage = BufferUsage::Default,
		const CPUAccess _cpu = CPUAccess::None) override;

	// サンプラー作成
	const Handle CreateSampler(const char* _samplerName, const SamplerDesc& _desc) override;

	// View作成
	const Handle CreateSRV(const Handle& _textureHandle, const char* name,
		const Format format, const uint16_t mostDetailedMip = 0, const int16_t mipLevels = -1) override;
	const Handle CreateRTV(const Handle& _textureHandle, const char* name, const uint16_t mipSlice) override;
	const Handle CreateDSV(const Handle& _textureHandle, const char* name, const Format format) override;

	// テクスチャのロード
	const TextureHandle LoadTexture(const char* textureName, const int16_t _mipLevels = -1, const char* textureFolderName = "") override;
	// モデルのロード
	const Handle LoadModel(const char* modelName, const char* modelFolderName = "") override;
};
