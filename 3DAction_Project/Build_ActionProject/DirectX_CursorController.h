#pragma once

// ==========================================
// 【クラス概要】
// カーソル制御を行うDirectX用クラスです。
// ==========================================


// ===========================================
// ヘッダー
// ===========================================
// 基底クラスのヘッダー
#include "CursorController.h"
// Windowsヘッダー
#include <Windows.h>


/// ==========================================
// クラス
// ==========================================
class DirectX_CursorController : public CursorController
{
private:
	// ウィンドウハンドルのポインター
	HWND* m_Hwnd;

	// モード切替時の処理
	void CursorModeChanged() override final;
	// ウィンドウから出ないようにする
	void ClipCursorToWindow();
	
	// カーソルの表示・非表示
	void SetCursorVisible(bool isShow);

public:
	// コンストラクタ・デストラクタ
	DirectX_CursorController(HWND* hwnd) : m_Hwnd(hwnd) {}
	~DirectX_CursorController() = default;

	// カーソル更新処理
	void Update();

	// カーソルを原点に戻す
	void ResetCursorPosition() override final;
};

