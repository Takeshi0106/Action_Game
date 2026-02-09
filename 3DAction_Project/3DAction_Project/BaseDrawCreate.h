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
// 文字列ヘッダー
#include "UTF8_String.h"
#include "Hashed_String.h"
// バイナリービューヘッダー
#include "BinaryView.h"


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
	virtual Handle CreateVertexBuffer(
		const Hashed_String& _vbName,
		const BinaryView& _vertexData,
		const uint32_t _vertexNumber,
		const PrimitiveType _type = PrimitiveType::TriangleStrip,
		const BufferUsage _usage = BufferUsage::Dynamic,
		const CPUAccess _access = CPUAccess::Write) = 0;

	// インデックスバッファ作成
	virtual Handle CreateIndexBuffer(
		const Hashed_String& _indexName,
		const uint32_t* _indexData,
		const size_t _indexSize,
		const uint32_t _indexNumber, 
		const BufferUsage _usage,
		const CPUAccess _access) = 0;

	// 定数バッファ作成
	virtual Handle CreateConstantBuffer(
		const Hashed_String& _constantName,
		const BinaryView& _data,
		const BufferUsage _usage = BufferUsage::Dynamic,
		const CPUAccess _access = CPUAccess::Write) = 0;

	// テクスチャ作成
	virtual Handle CreateTexture(
		const Hashed_String& _name,
		const uint16_t _width,
		const uint16_t _height,
		const Format _format,
		const BindFlag _bindFlag,
		const BufferUsage _usage = BufferUsage::Default,
		const CPUAccess _cpu = CPUAccess::None) = 0;

	// サンプラー作成_
	virtual Handle CreateSampler(const Hashed_String& _name, const SamplerDesc& _desc) = 0;

	// View作成
	// SRV
	virtual Handle CreateSRV(
		const Hashed_String& name,
		const Format format, 
		TextureHandle& _outTextureHandle,
		const uint16_t mostDetailedMip = 0, 
		const int16_t mipLevels = -1) = 0;
	// RTV
	virtual Handle CreateRTV(
		const Hashed_String& name, 
		const uint16_t mipSlice,
		TextureHandle& _outputTextureHandle) = 0;
	// DSV
	virtual Handle CreateDSV(
		const Hashed_String& name, 
		const Format format,
		TextureHandle& _outputTextureHandle) = 0;

	// テクスチャのロード
	virtual Handle LoadTexture(
		const Hashed_String& textureName, 
		TextureHandle& outTextureHandle,
		const int16_t _mipLevels = -1, 
		const String& textureFolderName = u8"") = 0;

	// モデルのロード
	virtual Handle LoadModel(const Hashed_String& modelName, const String& modelFolderName = u8"") = 0;
};
