#pragma once
// ==========================================
// ヘッダー
// ==========================================
// 整数ヘッダー
#include <cstdint>
// サンプラーヘッダー
#include "SamplerSetting.h"


// ==============================
// アクセス設定
// ==============================
// 自作のプリミティブタイプ
enum PrimitiveType : uint32_t
{
	TriangleList,
	TriangleStrip,
	LineList,
	LineStrip
};


// 自作の使用タイプ
enum BufferUsage : uint32_t
{
	// GPUのみアクセス
	Default,
	// CPUからも書き込み可能
	Dynamic,
	// CPUからも読み書き可能
	Staging
};


// 自作のCPUアクセス
enum CPUAccess : uint32_t
{
	None,
	Write
};


// 自作のフォーマット
enum Format : uint32_t
{
	Format_Unknown,

	// 8bit
	Format_R8G8B8A8_UNorm,
	Format_B8G8R8A8_UNorm_SRGB,

	// 単
	Format_R32_Float,

	// 高精度
	Format_R32G32B32_Float,
	Format_R32G32B32A32_Float,

	// 深度
	Format_D24_UNorm_S8_UInt
};


// ==========================================
// View バインドフラグ
// ==========================================
enum  BindFlag : uint32_t
{
	// バインドなし
	Bind_None = 0,
	// SRV
	Bind_ShaderResource = 1 << 0,
	// RTV
	Bind_RenderTarget = 1 << 1,
	// DSV
	Bind_DepthStencil = 1 << 2,
	// UAV (今は実装されていません)
	// Bind_UnorderedAccess = 1 << 3,
};

// OR演算子
inline BindFlag operator|(BindFlag a, BindFlag b)
{
	return static_cast<BindFlag>(
		static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
}


// ===========================================
// ミップマップ設定
// ===========================================
enum class MipMapType
{
	// ミップマップなし
	None,
	// 自動生成
	Auto,
	// 手動設定
	Manual
};


// ===========================================
// テクスチャ作成デスク
// ===========================================
// テクスチャ作成デスク
struct TextureCreateDesc
{
	// 基準テクスチャ(ウィンドウサイズ)との比率
	float baseRelativeScaleX = 1.0f;
	float baseRelativeScaleY = 1.0f;
	// 絶対サイズ
	uint32_t absoluteWidth;
	uint32_t absoluteHeight;

	// フォーマット
	Format format;
	BindFlag bindFlags;
	// CPU,GPUどちらが触るかのフラグ
	BufferUsage usage = BufferUsage::Default;
	// CPU から更新可能かフラグ
	CPUAccess cpuAccess = CPUAccess::None;

	// ミップマップ設定
	MipMapType mipMapType = MipMapType::None;
	// ミップマップレベル (手動設定時のみ有効)
	uint16_t mipLevels = 1;

	// サンプラー設定
	SamplerDesc sampler = SamplerDesc::NormalSampler();
};

// ===========================================
// テクスチャロード時の作成デスク
// のちに.DDSなどの特殊なフォーマットに対応させたい
// ===========================================
struct TextureLoadDesc
{
	// ミップを自動生成するかフラグ
	bool generateMip = false;
	// どのミップからロードするか 
	uint16_t firstMip = 0;
	// 読み込むミップ数
	int16_t mipCount = -1;
	
	// サンプラー設定
	SamplerDesc sampler = SamplerDesc::NormalSampler();
};
