#pragma once


// ==============================
// アクセス設定
// ==============================
// 自作のプリミティブタイプ
enum PrimitiveType
{
	TriangleList,
	TriangleStrip,
	LineList,
	LineStrip
};

// 自作の使用タイプ
enum BufferUsage
{
	Default,
	Dynamic
};

// 自作のCPUアクセス
enum CPUAccess
{
	None,
	Write
};

// 自作のフォーマット
enum Format
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
// View 関連
// ==========================================
// バインドフラグ(Viewやテクスチャ用)
enum  BindFlag : unsigned int
{
	Bind_None = 0,
	Bind_VertexBuffer = 1 << 0,
	Bind_IndexBuffer = 1 << 1,
	Bind_ConstantBuffer = 1 << 2,
	Bind_ShaderResource = 1 << 3,
	Bind_RenderTarget = 1 << 4,
	Bind_DepthStencil = 1 << 5,
	Bind_UnorderedAccess = 1 << 6,
};

// OR演算子
inline BindFlag operator|(BindFlag a, BindFlag b)
{
	return static_cast<BindFlag>(
		static_cast<unsigned int>(a) | static_cast<unsigned int>(b));
}


