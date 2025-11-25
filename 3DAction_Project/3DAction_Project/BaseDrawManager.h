#pragma once

// ===========================================================
// 【クラス概要】
// 描画マネージャーの抽象化クラス（基底クラス）
// このクラスを派生させてプラットフォーム別の
// 描画マネージャーを作成する
// ゲーム基盤とプラットフォーム描画マネージャーを繋ぐインターフェイスとしての
// 責任を持つ
// 
// 描画データは型削除 (type erasure) で渡す
// デバッグ時はサイズを比較してエラーが出せるようにする
// ===========================================================


// ==============================
// ヘッダー
// ==============================
// 設定用ヘッダー
#include "GraphicsEnums.h"
// サンプラー設定ヘッダー
#include "SamplerSetting.h"
// 各描画設定ヘッダー
#include "AlphaDizaSetting.h" // アルファディザ設定
#include "CullingSetting.h"   // カリング設定
#include "FillModeSetting.h"  // 塗り設定
#include "DepthStencilSetting.h" // 深度ステンシル設定
// 基本ヘッダー
#include <cstdint>


// ==============================
// クラス
// ==============================
class BaseDrawManager
{
protected:

public:
	// コンストラクタ・デストラクタ
	BaseDrawManager() = default;
	virtual ~BaseDrawManager() = default;

	// 描画
	virtual void BegingDraw() = 0;
	virtual void EndDraw() = 0;
	virtual void ModelDraw(const char* _vsShaderName,
		const char* _psShaderName,
		const char* _modelName) = 0;

	virtual void PrimitiveDraw(const char* _vsShaderName,
		const char* _psShaderName,
		const char* _vsBufferName,
		const char* _textureNam = nullptr,
		const SamplerDesc& _sampler = SamplerDesc::NormalSampler()) = 0;

	/* ------------ リソース作成 ------------ */
	// 頂点バッファ作成
	virtual bool CreateVertexBuffer(
		const char* modelName, 
		const void* data, 
		size_t size,
		uint32_t vertexNumber,
		uint32_t maxNumber,
		PrimitiveType type = PrimitiveType::TriangleStrip,
		BufferUsage usage = BufferUsage::Dynamic,
		CPUAccess access = CPUAccess::Write) = 0;

	// インデックスバッファ作成
	virtual bool CreateIndexBuffer(
		const char* modelName,
		const uint32_t* indexData,
		uint32_t indexNumber) = 0;

	// 定数バッファ作成
	virtual bool CreateConstantBuffer(
		const char* constantName,
		const void* data,
		size_t size,
		BufferUsage usage = BufferUsage::Dynamic,
		CPUAccess access = CPUAccess::Write) = 0;

	// テクスチャ作成
	virtual bool CreateTexture(
		const char* name,
		unsigned int width,
		unsigned int height,
		Format format,
		BindFlag bindFlag,
		BufferUsage usage = BufferUsage::Default,
		CPUAccess cpu = CPUAccess::None) = 0;

	// テクスチャのロード
	virtual bool LoadTexture(const char* textureName) = 0;
	// モデルのロード
	virtual bool LoadModel(const char* modelName, const char* modelFolderName = "") = 0;

	// サンプラー作成
	virtual bool CreateSampler(const SamplerDesc& _desc) = 0;

	// View作成
	virtual bool CreateSRV(const char* name, Format format, unsigned int mostDetailedMip = 0, unsigned int mipLevels = -1) = 0;
	virtual bool CreateRTV(const char* name, uint32_t mipSlice) = 0;
	virtual bool CreateDSV(const char* name, Format format) = 0;

	//virtual bool CreateUAV(const char* name, Format format, unsigned int mipSlice = 0) = 0;


	/* ------------ バッファ更新 ------------ */
	// 頂点バッファ更新
	virtual void UpdateVertexBuffer(const char* vertexName, const void* data, int size) = 0;
	// 定数バッファ更新
	virtual void UpdateShaderConstants(const char* constantName, const void* data, const int size) = 0;

	// バインドレンダーターゲット
	virtual void BindRenderTarget(const char* rtvName, const char* dsvName = nullptr) = 0;


	/* ------------ 描画設定 ------------ */
	// 描画設定(カリング、塗り)
	virtual void SetDrawSetting(CullingSetting culling = CullingSetting::Back_Culling,
		FillModeSetting fillMode = FillModeSetting::Solid) = 0;

	// 深度ステンシル設定
	virtual void SetDepthStencilSetting(DepthStencilSetting depthStencil 
		= DepthStencilSetting::DepthEnableON_DepthWriteON) = 0;

	// アルファディザ設定
	virtual void SetAlphaDizaSetting(AlphaDizaSetting alphaDiza 
		= AlphaDizaSetting::Blend_Alpha) = 0;
};

