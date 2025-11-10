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
#include "GraphicsEnums.h" // 設定用ヘッダー


// ==============================
// クラス
// ==============================
class BaseDrawManager
{
private:

public:
	// コンストラクタ・デストラクタ
	BaseDrawManager() = default;
	virtual ~BaseDrawManager() = default;

	// 描画
	virtual void BegingDraw() = 0;
	virtual void EndDraw() = 0;
	virtual void Draw(const char* id, const void* data, const int size) = 0;

	virtual void Draw(const char* _vsShaderName,
		const char* _psShaderName,
		const char* _textureNam = nullptr,
		const char* _modelName = nullptr,
		const SamplerDesc& _sampler = SamplerDesc::NormalSampler()) = 0;

	/* ------------ リソース作成 ------------ */
	// 頂点バッファ作成
	virtual bool CreateVertexBuffer(
		const char* drawID, 
		const void* data, 
		size_t size,
		int vertexNumber,
		PrimitiveType type = PrimitiveType::TriangleStrip,
		BufferUsage usage = BufferUsage::Dynamic,
		CPUAccess access = CPUAccess::Write) = 0;

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

	// サンプラー作成
	virtual bool CreateSampler(const SamplerDesc& _desc) = 0;

	// View作成
	virtual bool CreateSRV(const char* name, Format format, unsigned int mostDetailedMip = 0, unsigned int mipLevels = -1) = 0;
	virtual bool CreateUAV(const char* name, Format format, unsigned int mipSlice = 0) = 0;
	virtual bool CreateRTV(const char* name, Format format, unsigned int mipSlice = 0) = 0;
	virtual bool CreateDSV(const char* name, Format format, unsigned int mipSlice = 0) = 0;


	/* ------------ バッファ更新 ------------ */
	// 頂点バッファ更新
	virtual void UpdateVertexBuffer(const char* vertexName, const void* data, int size) = 0;
	// 定数バッファ更新
	virtual void UpdateShaderConstants(const char* constantName, const void* data, const int size) = 0;
};

