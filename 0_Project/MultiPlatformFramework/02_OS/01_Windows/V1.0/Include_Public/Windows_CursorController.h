#pragma once

// ==========================================
// 【クラス概要】
// カーソル制御を行うWindows用クラスです。
// ==========================================

// ===========================================
// ヘッダー
// ===========================================
// 基底クラスのヘッダー
#include "CursorController.h"
// Windowsヘッダー
#include "WindowsHandle.h"

/// ==========================================
// クラス
// ==========================================
class Windows_CursorController : public CursorController
{
private:
	// ウィンドウハンドルのポインター
	Windows_Handle* m_pWindowsHandle;

	// モード切替時の処理
	void CursorModeChanged() override final;
	// ウィンドウから出ないようにする
	void ClipCursorToWindow();
	
	// カーソルの表示・非表示
	void SetCursorVisible(bool isShow);

public:
	// コンストラクタ・デストラクタ
	Windows_CursorController(Windows_Handle* pWindowsHandle) : m_pWindowsHandle(pWindowsHandle) {}
	~Windows_CursorController() = default;

	// カーソル更新処理
	void Update();

	// カーソルを原点に戻す
	void ResetCursorPosition() override final;
};

