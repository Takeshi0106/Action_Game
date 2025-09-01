#pragma once

// ==============================
// バッファ設定用
// ==============================


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
