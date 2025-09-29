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
	// リソースマネージャー
	std::unique_ptr<ShaderManager> m_ShaderManager;     // シェーダーマネージャー
	std::unique_ptr<ConstantBufferManager> m_CBManager; // 定数バッファマネージャー
	std::unique_ptr<VertexBufferManager> m_VBManager; // 頂点バッファマネージャー
	std::unique_ptr<TextureManager> m_TextureManager; // テクスチャマネージャー



public:
	// コンストラクタ
	DirectX_DrawManager();
	~DirectX_DrawManager();

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


	// デバッグ処理（描画できるかのチェックのため後に削除）
	// デバッグ用更新
	void DebugUpdate();
	// デバッグ用描画
	void DebugDraw();
};

