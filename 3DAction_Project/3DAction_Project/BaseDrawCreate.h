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
// グラフィックス列挙型
#include "../../GraphicsEnums.h"
// ハンドル設定
#include "Handle.h"
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
private:
	// --------------------------------
	// メンバー変数
	// --------------------------------
	uint32_t m_ScreenWidth = 0;
	uint32_t m_ScreenHeight = 0;

public:
	// --------------------------------
	// コンストラクタ・デストラクタ
	// --------------------------------
	BaseDrawCreate(uint32_t& _width,uint32_t& _height) :
		m_ScreenWidth(_width), m_ScreenHeight(_height) {
	}
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

	// テクスチャの作成
	virtual Handle CreateTexture(
		const Hashed_String& textureName,
		const TextureCreateDesc& textureDesc) = 0;

	// テクスチャのロード
	virtual Handle LoadTexture(
		const Hashed_String& textureName, 
		const String& textureFolderName = u8"") = 0;

	// モデルのロード
	virtual Handle LoadModel(const Hashed_String& modelName, const String& modelFolderName = u8"") = 0;
};
