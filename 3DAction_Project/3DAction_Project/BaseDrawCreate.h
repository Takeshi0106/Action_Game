#pragma once

// ===============================================
// クラス概要
// 抽象化したリソース作成関数
// ===============================================


// ===============================================
// ヘッダー
// ===============================================
// 設定用ヘッダー
#include "GraphicsEnums.h"
// サンプラー設定ヘッダー
#include "SamplerSetting.h"
// ハンドル設定
#include "Handle.h"
// テクスチャハンドル設定
#include "TextureHandleh.h"
// モデルハンドル設定
#include "ModelHandle.h"
// 固定長整数ヘッダー
#include <cstdint>


// ===============================================
// クラス
// ===============================================
class BaseDrawCreate
{
public:
	// --------------------------------
	// コンストラクタ・デストラクタ
	// --------------------------------
	BaseDrawCreate() = default;
	virtual ~BaseDrawCreate() = default;


	// --------------------------------
	// リソース作成
	// --------------------------------
	// 頂点バッファ作成
	virtual const Handle CreateVertexBuffer(
		const char* _vbName,
		const void* _data,
		const size_t _size,
		const uint32_t _vertexNumber,
		const PrimitiveType _type = PrimitiveType::TriangleStrip,
		const BufferUsage _usage = BufferUsage::Dynamic,
		const CPUAccess _access = CPUAccess::Write) = 0;

	// インデックスバッファ作成
	virtual const Handle CreateIndexBuffer(
		const char* _indexName,
		const uint32_t* _indexData,
		const size_t _indexSize,
		const uint32_t _indexNumber, 
		const BufferUsage _usage,
		const CPUAccess _access) = 0;

	// 定数バッファ作成
	virtual const Handle CreateConstantBuffer(
		const char* _constantName,
		const size_t _size,
		const void* _data = nullptr,
		const BufferUsage _usage = BufferUsage::Dynamic,
		const CPUAccess _access = CPUAccess::Write) = 0;

	// テクスチャ作成
	virtual const Handle CreateTexture(
		const char* _name,
		const uint16_t _width,
		const uint16_t _height,
		const Format _format,
		const BindFlag _bindFlag,
		const BufferUsage _usage = BufferUsage::Default,
		const CPUAccess _cpu = CPUAccess::None) = 0;

	// サンプラー作成
	virtual const Handle CreateSampler(const char* _samplerName, const SamplerDesc& _desc) = 0;

	// View作成
	virtual const Handle CreateSRV(const Handle& _textureHandle, const char* name,
		const Format format, const uint16_t mostDetailedMip = 0, const int16_t mipLevels = -1) = 0;
	virtual const Handle CreateRTV(const Handle& _textureHandle, const char* name, const uint16_t mipSlice) = 0;
	virtual const Handle CreateDSV(const Handle& _textureHandle, const char* name, const Format format) = 0;

	// テクスチャのロード
	virtual const TextureHandle LoadTexture(const char* textureName, const int16_t _mipLevels = -1, const char* textureFolderName = "") = 0;
	// モデルのロード
	virtual const Handle LoadModel(const char* modelName, const char* modelFolderName = "") = 0;
};
