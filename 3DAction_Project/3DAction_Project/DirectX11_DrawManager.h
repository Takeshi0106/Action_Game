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
// 色設定
#include "Color.h"
// 標準ライブラリ
#include <cstdint>

// マネージャーヘッダー
#include "ShaderManager.h" // シェーダーマネージャー
#include "ConstantBufferManager.h" // 定数バッファマネージャー
#include "VertexBufferManager.h"   // 頂点バッファマネージャー
#include "TextureManager.h" // テクスチャマネージャー
#include "ResourceViewManager.h" // ビューマネージャー
#include "SamplerManager.h" // サンプラーマネージャー
#include "IndexBufferManager.h" // インデックスバッファマネージャー
#include "ModelManager.h" // モデルマネージャー
// モジュール
#include "TextureLoader.h"
#include "ModelConversionModule.h"
// コンフィグ
#include "DrawPathConfig.h"


// ========================================
// 前方宣言
// ========================================
// ウィンドウハンドルの前方宣言
struct HWND__;
using HWND = HWND__*;
// DirectXの前方宣言
struct ID3D11Device;
struct ID3D11DeviceContext;


// ========================================
// DirectXの描画マネージャー
// ========================================
class DirectX_DrawManager final : public BaseDrawManager
{
private:
	// --------------------------------
	// メンバー変数
	// --------------------------------

	// 最終描画に使用するRTの名前
	const String kFinalRTName = u8"FinalRT";
	const String kFInalDSName = u8"FinalDS";
	// クリアカラー
	const Color kClearColor = Color(0.1f, 0.3f, 0.7f, 1.0f);


	// --------------------------------
	// リソースマネージャー
	// --------------------------------
	// シェーダーマネージャー
	ShaderManager m_ShaderManager;
	// 定数バッファマネージャー
	ConstantBufferManager m_CBManager;
	// 頂点バッファマネージャー
	VertexBufferManager m_VBManager;
	// テクスチャマネージャー
	TextureManager m_TextureManager;
	// ビューマネージャー
	ResourceViewManager m_ViewManager;
	// サンプラーマネージャー
	SamplerManager m_SamplerManager;
	// インデックスバッファ
	IndexBufferManager m_IndexBufferManager;
	// モデルマネージャー
	ModelManager m_ModelManager;


	// --------------------------------
	// モジュール
	// --------------------------------
	// テクスチャをロードするモジュール
	TextureLoader m_TextureLoader;
	// モデル変換モジュール
	ModelConversionModule m_ModelConversionModule;


	// --------------------------------
	// 描画コマンド
	// --------------------------------
	// 描画
	bool DrawModelObject(
		const String& _vsShaderName,
		const String& _psShaderName,
		const String& _modelName);

	// インデックスバッファを使用したメッシュ描画
	bool DrawIndexObject(
		const String& _vsShaderName,
		const String& _psShaderName,
		const String& _vbName,
		const String& _ibName,
		const String& _textureName,
		const SamplerDesc _sampler);

	bool DrawPrimitiveObject(
		const String& _vsShaderName,
		const String& _psShaderName,
		const String& _vsBufferName,
		const String& _textureName, 
		const SamplerDesc _desc);

public:
	// コンストラクタ
	DirectX_DrawManager(const DrawPathConfig& _config);
	~DirectX_DrawManager();

	// 初期化
	bool Init(uint16_t Width, uint16_t Height, HWND windowHandle);
	// 後処理
	void Uninit();

	// 描画
	void BegingDraw();
	void EndDraw();

	// モデル描画
	void ModelDraw(
		const String& _vsShaderName, 
		const String& _psShaderName, 
		const String& _modelName) override final;

	// インデックスバッファを使用したメッシュ描画
	void IndexedDraw(
		const String& _vsShaderName,
		const String& _psShaderName,
		const String& _vbName,
		const String& _ibName,
		const String& _textureName = u8"",
		const SamplerDesc& _sampler = SamplerDesc::NormalSampler()) override final;
	
	// プリミティブ描画
	void PrimitiveDraw(
		const String& _vsShaderName,
		const String& _psShaderName,
		const String& _vsBufferName,
		const String& _textureName = u8"",
		const SamplerDesc& _sampler = SamplerDesc::NormalSampler()) override final;

	// 頂点バッファ作成
	bool CreateVertexBuffer(
		const String& modelName,
		const void* data,
		size_t size,
		uint32_t vertexNumber,
		uint32_t maxNumber,
		PrimitiveType type,
		BufferUsage usage,
		CPUAccess access) override final;

	// インデックスバッファ作成
	bool CreateIndexBuffer(
		const String& modelName,
		const uint32_t* indexData,
		uint32_t indexNumber) override final;

	// 定数バッファ作成
	bool CreateConstantBuffer(
		const String& constantName,
		const void* data,
		size_t size,
		BufferUsage usage = BufferUsage::Dynamic,
		CPUAccess access = CPUAccess::Write) override final;

	// テクスチャ作成
	bool CreateTexture(
		const String& name,
		uint32_t width,
		uint32_t height,
		Format format,
		BindFlag bindFlag,
		BufferUsage usage = BufferUsage::Default,
		CPUAccess cpu = CPUAccess::None) override final;

	// サンプラー作成
	bool CreateSampler(const SamplerDesc& _desc) override final;

	// テクスチャのロード
	bool LoadTexture(const String& textureName) override final;
	// モデルのロード
	bool LoadModel(const String& modelName, const String& modelFolderName = u8"") override final;

	// View作成
	bool CreateSRV(const String& name, Format format, unsigned int mostDetailedMip = 0, unsigned int mipLevels = -1) override final;
	bool CreateRTV(const String& name, uint32_t mipSlice) override final;
	bool CreateDSV(const String& name, Format format) override final;

	//bool CreateUAV(const char* name, Format format, unsigned int mipSlice = 0) override;
	
	// バッファ更新
	// 定数バッファ更新
	void UpdateShaderConstants(const String& constantName, const void* data, const int size) override final;
	// 頂点バッファ更新
	void UpdateVertexBuffer(const String& vertexName, const void* data, int size) override final;

	// レンダーターゲットバインド
	void BindRenderTarget(const String& rtvName = u8"", const String& dsvName = u8"") override final;

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
