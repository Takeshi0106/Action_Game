#pragma once

// ========================================
// 【クラス概要】
// DirectX描画マネージャー
// ========================================


// ========================================
// ヘッダー
// ========================================
#include "BaseDrawManager.h"


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


// ========================================
// DirectXの描画マネージャー
// ========================================
class DirectX_DrawManager : public BaseDrawManager
{
private:
	static ShaderManager m_ShaderManager;     // シェーダーマネージャー
	static ConstantBufferManager m_CBManager; // 定数バッファマネージャー
	static VertexBufferManager m_VBManager; // 頂点バッファマネージャー
	static TextureManager m_TextureManager;

public:
	// 初期化
	bool Init(unsigned int Width, unsigned int Height, HWND windowHandle);
	// 後処理
	void Uninit();

	// 描画
	void Draw(const char* drawID, const void* data, const int size) override;

	// 頂点バッファ作成
	void CreateVertexBuffer(
		const char* drawID,
		const void* data,
		size_t size,
		PrimitiveType type,
		BufferUsage usage,
		CPUAccess access) override;

	// 定数バッファ作成
	void CreateConstantBuffer(
		const char* constantName,
		const void* data,
		size_t size,
		BufferUsage usage = BufferUsage::Dynamic,
		CPUAccess access = CPUAccess::Write) override;

	// バッファ更新
	// 定数バッファ更新
	void UpdateShaderConstants(const char* constantName, const void* data, const int size) override;
	// 頂点バッファ更新
	void UpdateVertexBuffer(const char* drawID, const void* data, int size) override;

	// デバッグ用更新
	void DebugUpdate();
	// デバッグ用描画
	void DebugDraw();
};

