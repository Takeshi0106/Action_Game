#pragma once


// ==============================
// ヘッダー
// ==============================
#include "SamplerSetting.h"


// ==============================
// アクセス設定
// ==============================
// 自作のプリミティブタイプ
enum class PrimitiveType
{
	TriangleList,
	TriangleStrip,
	LineList,
	LineStrip
};

// 自作の使用タイプ
enum class BufferUsage
{
	Default,
	Dynamic
};

// 自作のCPUアクセス
enum class CPUAccess
{
	None,
	Write
};

// 自作のフォーマット
enum class Format
{
	Unknown,

	// 8bit
	R8G8B8A8_UNorm,
	B8G8R8A8_UNorm_SRGB,

	// 単
	R32_Float,

	// 高精度
	R32G32B32_Float,
	R32G32B32A32_Float,

	// 深度
	D24_UNorm_S8_UInt
};


// ==========================================
// View 関連
// ==========================================
// バインドフラグ(Viewやテクスチャ用)
enum class BindFlag : unsigned int
{
	None = 0,
	VertexBuffer = 1 << 0,
	IndexBuffer = 1 << 1,
	ConstantBuffer = 1 << 2,
	ShaderResource = 1 << 3,
	RenderTarget = 1 << 4,
	DepthStencil = 1 << 5,
	UnorderedAccess = 1 << 6,
};

// OR演算子
inline BindFlag operator|(BindFlag a, BindFlag b)
{
	return static_cast<BindFlag>(
		static_cast<unsigned int>(a) | static_cast<unsigned int>(b));
}


