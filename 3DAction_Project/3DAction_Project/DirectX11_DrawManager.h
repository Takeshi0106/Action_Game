#pragma once

// ========================================
// 【クラス概要】
// DirectX描画マネージャー
// 
// 【クラスの責任】
// 窓口の責任
// 描画時の最適化各リソースにバインド・更新命令、各リソースマネージャーへの窓口
// 各リソースのバインド順などを決める
// ========================================


// ========================================
// ヘッダー
// ========================================
// 抽象化描画マネージャー(基底クラス)
#include "BaseDrawManager.h"
// std::unique_ptrを使用するため
#include <memory>
// 色設定
#include "Color.h"
// 標準ライブラリ
#include <cstdint>


// ========================================
// 前方宣言
// ========================================
// ウィンドウハンドルの前方宣言
struct HWND__;
using HWND = HWND__*;
// マネージャーの前方宣言
class ShaderManager;
class ConstantBufferManager;
class VertexBufferManager;
class TextureManager;
class ResourceViewManager;	
class SamplerManager;
class IndexBufferManager;
class ModelManager;
// モジュールの前方宣言
class TextureLoader;
class ModelConversionModule;
// DirectXの前方宣言
struct ID3D11Device;
struct ID3D11DeviceContext;


// ========================================
// DirectXの描画マネージャー
// ========================================
class DirectX_DrawManager : public BaseDrawManager
{
private:
	// 画面の幅・高さ
	uint16_t m_Width = 0;
	uint16_t m_Height = 0;

	// 最終描画に使用するRTの名前
	const char* kFinalRTName = "FinalRT";
	const char* kFInalDSName = "FinalDS";
	const Color kClearColor = Color(0.1f, 0.3f, 0.7f, 1.0f); // クリアカラー

	// リソースマネージャー
	std::unique_ptr<ShaderManager> m_ShaderManager;     // シェーダーマネージャー
	std::unique_ptr<ConstantBufferManager> m_CBManager; // 定数バッファマネージャー
	std::unique_ptr<VertexBufferManager> m_VBManager; // 頂点バッファマネージャー
	std::unique_ptr<TextureManager> m_TextureManager; // テクスチャマネージャー
	std::unique_ptr<ResourceViewManager> m_ViewManager; // ビューマネージャー
	std::unique_ptr<SamplerManager> m_SamplerManager; // サンプラーマネージャー
	std::unique_ptr<IndexBufferManager> m_IndexBufferManager; // インデックスバッファ
	std::unique_ptr<ModelManager> m_ModelManager; // モデルマネージャー

	// モジュール
	std::unique_ptr<TextureLoader> m_TextureLoader; // テクスチャをロードするモジュール
	std::unique_ptr<ModelConversionModule> m_ModelConversionModule; // モデル変換モジュール

	// 描画
	bool DrawModelObject(const char* _vsShaderName,
		const char* _psShaderName,
		const char* _modelName);

	bool DrawPrimitiveObject(const char* _vsShaderName,
		const char* _psShaderName,
		const char* _vsBufferName,
		const char* _textureName, 
		const SamplerDesc _desc);

public:
	// コンストラクタ
	DirectX_DrawManager();
	~DirectX_DrawManager();

	// 初期化
	bool Init(uint16_t Width, uint16_t Height, HWND windowHandle);
	// 後処理
	void Uninit();

	// 描画
	void BegingDraw();
	void EndDraw();

	// モデル描画
	void ModelDraw(const char* _vsShaderName, 
		const char* _psShaderName, 
		const char* _modelName) override final;

	void PrimitiveDraw(const char* _vsShaderName,
		const char* _psShaderName,
		const char* _vsBufferName,
		const char* _textureName = nullptr,
		const SamplerDesc& _sampler = SamplerDesc::NormalSampler()) override final;

	// 頂点バッファ作成
	bool CreateVertexBuffer(
		const char* modelName,
		const void* data,
		size_t size,
		uint32_t vertexNumber,
		uint32_t maxNumber,
		PrimitiveType type,
		BufferUsage usage,
		CPUAccess access) override final;

	// インデックスバッファ作成
	bool CreateIndexBuffer(
		const char* modelName,
		const uint32_t* indexData,
		uint32_t indexNumber) override final;

	// 定数バッファ作成
	bool CreateConstantBuffer(
		const char* constantName,
		const void* data,
		size_t size,
		BufferUsage usage = BufferUsage::Dynamic,
		CPUAccess access = CPUAccess::Write) override final;

	// テクスチャ作成
	bool CreateTexture(
		const char* name,
		uint32_t width,
		uint32_t height,
		Format format,
		BindFlag bindFlag,
		BufferUsage usage = BufferUsage::Default,
		CPUAccess cpu = CPUAccess::None) override final;

	// サンプラー作成
	bool CreateSampler(const SamplerDesc& _desc) override final;

	// テクスチャのロード
	bool LoadTexture(const char* textureName) override final;
	// モデルのロード
	bool LoadModel(const char* modelName, const char* modelFolderName = "") override final;

	// View作成
	bool CreateSRV(const char* name, Format format, unsigned int mostDetailedMip = 0, unsigned int mipLevels = -1) override final;
	bool CreateRTV(const char* name, uint32_t mipSlice) override final;
	bool CreateDSV(const char* name, Format format) override final;

	//bool CreateUAV(const char* name, Format format, unsigned int mipSlice = 0) override;
	
	// バッファ更新
	// 定数バッファ更新
	void UpdateShaderConstants(const char* constantName, const void* data, const int size) override final;
	// 頂点バッファ更新
	void UpdateVertexBuffer(const char* vertexName, const void* data, int size) override final;

	// レンダーターゲットバインド
	void BindRenderTarget(const char* rtvName = nullptr, const char* dsvName = nullptr) override final;

	/* ------------ 描画設定 ------------ */
	// 描画設定(カリング、塗り)
	void SetDrawSetting(FillModeSetting fillMode = FillModeSetting::Solid,
		CullingSetting culling = CullingSetting::Back_Culling) override final;

	// 深度ステンシル設定
	void SetDepthStencilSetting(DepthStencilSetting depthStencil
		= DepthStencilSetting::DepthEnableON_DepthWriteON) override final;

	// アルファディザ設定
	void SetAlphaDizaSetting(AlphaDizaSetting alphaDiza
		= AlphaDizaSetting::Blend_Alpha) override final;


	// ゲッター
	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();
};
