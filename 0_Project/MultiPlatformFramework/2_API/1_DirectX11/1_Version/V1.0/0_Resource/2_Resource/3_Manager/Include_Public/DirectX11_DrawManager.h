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
#include "IDrawManager.h"
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
class DirectX_DrawManager final : public IDrawManager
{
private:
	// --------------------------------
	// メンバー変数
	// --------------------------------
	// 最終描画に使用するRTの名前
	const Hashed_String kFinalRTName = Hashed_String(u8"FinalRT");
	const Hashed_String kFInalDSName = Hashed_String(u8"FinalDS");
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
		const Hashed_String& _vsShaderName,
		const Hashed_String& _psShaderName,
		const Hashed_String& _modelName);

	// インデックスバッファを使用したメッシュ描画
	bool DrawIndexObject(
		const Hashed_String& _vsShaderName,
		const Hashed_String& _psShaderName,
		const Hashed_String& _vbName,
		const Hashed_String& _ibName,
		const Hashed_String& _textureName,
		const SamplerDesc _sampler);

	bool DrawPrimitiveObject(
		const Hashed_String& _vsShaderName,
		const Hashed_String& _psShaderName,
		const Hashed_String& _vsBufferName,
		const Hashed_String& _textureName, 
		const SamplerDesc _desc);

public:
	// コンストラクタ
	DirectX_DrawManager(const DrawPathConfig& _config);
	~DirectX_DrawManager();

	// 初期化
	bool Init(uint32_t Width, uint32_t Height, IWindowHandle* windowHandle) override final;
	// 後処理
	void Uninit() override final;

	// 描画
	void BegingDraw() override final;
	void EndDraw() override final;

	// モデル描画
	void ModelDraw(
		const Hashed_String& _vsShaderName, 
		const Hashed_String& _psShaderName, 
		const Hashed_String& _modelName) override final;

	// インデックスバッファを使用したメッシュ描画
	void IndexedDraw(
		const Hashed_String& _vsShaderName,
		const Hashed_String& _psShaderName,
		const Hashed_String& _vbName,
		const Hashed_String& _ibName,
		const Hashed_String& _textureName = Hashed_String(u8""),
		const SamplerDesc& _sampler = SamplerDesc::NormalSampler()) override final;
	
	// プリミティブ描画
	void PrimitiveDraw(
		const Hashed_String& _vsShaderName,
		const Hashed_String& _psShaderName,
		const Hashed_String& _vsBufferName,
		const Hashed_String& _textureName = Hashed_String(u8""),
		const SamplerDesc& _sampler = SamplerDesc::NormalSampler()) override final;

	// 頂点バッファ作成
	bool CreateVertexBuffer(
		const Hashed_String& modelName,
		const void* data,
		size_t size,
		uint32_t vertexNumber,
		uint32_t maxNumber,
		PrimitiveType type,
		BufferUsage usage,
		CPUAccess access) override final;

	// インデックスバッファ作成
	bool CreateIndexBuffer(
		const Hashed_String& modelName,
		const uint32_t* indexData,
		uint32_t indexNumber) override final;

	// 定数バッファ作成
	bool CreateConstantBuffer(
		const Hashed_String& constantName,
		const void* data,
		size_t size,
		BufferUsage usage = BufferUsage::Dynamic,
		CPUAccess access = CPUAccess::Write) override final;

	// テクスチャ作成
	bool CreateTexture(
		const Hashed_String& name,
		uint32_t width,
		uint32_t height,
		Format format,
		BindFlag bindFlag,
		BufferUsage usage = BufferUsage::Default,
		CPUAccess cpu = CPUAccess::None) override final;

	// サンプラー作成
	bool CreateSampler(const SamplerDesc& _desc) override final;

	// テクスチャのロード
	bool LoadTexture(const Hashed_String& textureName) override final;
	// モデルのロード
	bool LoadModel(const Hashed_String& modelName, const String& modelFolderName = u8"") override final;

	// View作成
	bool CreateSRV(const Hashed_String& name, Format format, unsigned int mostDetailedMip = 0, unsigned int mipLevels = -1) override final;
	bool CreateRTV(const Hashed_String& name, uint32_t mipSlice) override final;
	bool CreateDSV(const Hashed_String& name, Format format) override final;

	//bool CreateUAV(const char* name, Format format, unsigned int mipSlice = 0) override;
	
	// バッファ更新
	// 定数バッファ更新
	void UpdateShaderConstants(const Hashed_String& constantName, const void* data, const int size) override final;
	// 頂点バッファ更新
	void UpdateVertexBuffer(const Hashed_String& vertexName, const void* data, int size) override final;

	// レンダーターゲットバインド
	void BindRenderTarget(const Hashed_String& rtvName = Hashed_String(u8""), 
		const Hashed_String& dsvName = Hashed_String(u8"")) override final;

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
