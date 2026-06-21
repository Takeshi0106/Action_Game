#pragma once

// ===================================
// 【クラス概要】
// Windows のウィンドウハンドルの抽象化クラス
// ===================================

// ヘッダー
#include "IWindowHandle.h"
// Windowsのウィンドウハンドルを使用するためのヘッダー
#include <Windows.h>

class Windows_Handle final : public IWindowHandle
{
private:
	// Windowsのウィンドウハンドル
	HWND m_hWnd = nullptr;

public:
	// コンストラクタ・デストラクタ
	explicit Windows_Handle(HWND hWnd) : m_hWnd(hWnd){}
	~Windows_Handle() = default;

	// ゲッター
	void* GetWindowHandle() const override { return reinterpret_cast<void*>(m_hWnd); }
};
