#pragma once

// ===============================================
// クラス概要
// DirectX11用リソース作成クラス
// ===============================================


// ===============================================
// ヘッダー
// ===============================================
// リソース管理ヘッダー

// 基底ヘッダー
#include "BaseDrawCreate.h"


// ===============================================
// クラス
// ===============================================
class DirectX11_DrawCreate final : public BaseDrawCreate
{
private:

public:
	// --------------------------------
	// コンストラクタ・デストラクタ
	// --------------------------------
	DirectX11_DrawCreate() = default;
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
		const uint32_t _indexNumber) override;
	
	// 定数バッファ作成
	const Handle CreateConstantBuffer(
		const char* _constantName,
		const void* _data,
		const size_t _size,
		const BufferUsage _usage = BufferUsage::Dynamic,
		const CPUAccess _access = CPUAccess::Write) override;

	// テクスチャ作成
	const Handle CreateTexture(
		const char* _name,
		const uint32_t _width,
		const uint32_t _height,
		const Format _format,
		const BindFlag _bindFlag,
		const BufferUsage _usage = BufferUsage::Default,
		const CPUAccess _cpu = CPUAccess::None) override;

	// サンプラー作成
	const Handle CreateSampler(const char* _samplerName, const SamplerDesc& _desc) override;

	// View作成
	const Handle CreateSRV(const Handle& _textureHandle, const char* name,
		const Format format, const uint32_t mostDetailedMip = 0, const int32_t mipLevels = -1) override;
	const Handle CreateRTV(const Handle& _textureHandle, const char* name, const uint32_t mipSlice) override;
	const Handle CreateDSV(const Handle& _textureHandle, const char* name, const Format format) override;

	// テクスチャのロード
	const TextureHandle LoadTexture(const char* textureName, const char* textureFolderName = "") override;
	// モデルのロード
	const ModelHandle LoadModel(const char* modelName, const char* modelFolderName = "") override;
};

