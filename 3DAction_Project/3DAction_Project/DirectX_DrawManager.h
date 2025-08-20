#pragma once

// ========================================
//
// ========================================


// ========================================
// 前方宣言
// ========================================
// ウィンドウハンドルの前方宣言
struct HWND__;
using HWND = HWND__*;
// マネージャーの前方宣言
class ShaderManager;
class ConstantBufferManager;


// ========================================
// DirectXの描画マネージャー
// ========================================
class DirectX_DrawManager
{
private:
	static ShaderManager m_ShaderManager; // シェーダーマネージャー
	static ConstantBufferManager m_CBManager; // 定数バッファマネージャー

public:
	bool Init(unsigned int Width, unsigned int Height, HWND windowHandle); // 初期化
	void DebugDraw(); // デバッグ用描画

	void Uninit();

};

