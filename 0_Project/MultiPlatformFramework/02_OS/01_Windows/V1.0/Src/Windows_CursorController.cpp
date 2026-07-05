#include "Windows_CursorController.h"


// ===========================
// モード切替時の処理
// ===========================
void Windows_CursorController::CursorModeChanged()
{
	switch (m_CurrentMode)
	{
	// 通常モード
	case CursorMode_Normal:
		SetCursorVisible(true); // カーソル表示
		ClipCursor(nullptr); // カーソル固定解除
		break;

	// カーソル非表示モード
	case CursorMode_Hidden:
		SetCursorVisible(false); // カーソル非表示
		ClipCursor(nullptr); // カーソル固定解除
		break;

	// カーソル固定・非表示モード
	case CursorMode_Locked:
		SetCursorVisible(false); // カーソル非表示
		ClipCursorToWindow(); // ウィンドウから出ないようにする
		break;

		// カーソル固定・非表示・中央に戻すモード
	case CursorMode_LockedCenter:
		SetCursorVisible(false); // カーソル非表示
		ClipCursorToWindow(); // ウィンドウから出ないようにする
		ResetCursorPosition(); // カーソルを中央に戻す
		break;

	}
}


// ===========================
// カーソル更新処理
// ===========================
void Windows_CursorController::Update()
{
	// ロック・中央モードの場合は常に中央に戻す
	if (m_CurrentMode == CursorMode_LockedCenter)
	{
		// カーソルを中央に戻す
		ResetCursorPosition();
	}
}


// ===========================
// カーソルを原点に戻す
// ===========================
void Windows_CursorController::ResetCursorPosition()
{
	// HWMD に変換
	HWND windowHandle = static_cast<HWND>(m_pWindowsHandle->GetWindowHandle());

	// ウィンドウの大きさ取得
	RECT rect;
	GetClientRect(windowHandle, &rect);

	// スクリーン座標に変換
	MapWindowPoints(windowHandle, nullptr,
		reinterpret_cast<POINT*>(&rect), 2);

	// 中央を計算して移動
	SetCursorPos((rect.left + rect.right) / 2, (rect.top + rect.bottom) / 2);
}


// ===========================
// ウィンドウから出ないようにする
// ===========================
void Windows_CursorController::ClipCursorToWindow()
{
	// HWMD に変換
	HWND windowHandle = static_cast<HWND>(m_pWindowsHandle->GetWindowHandle());

	// ウィンドウの大きさ取得
	RECT rect;
	GetClientRect(windowHandle, &rect);

	// スクリーン座標に変換
	MapWindowPoints(windowHandle, nullptr,
		reinterpret_cast<POINT*>(&rect), 2);

	// カーソル固定
	ClipCursor(&rect);
}

// ===========================
// カーソルの表示・非表示
// ===========================
void Windows_CursorController::SetCursorVisible(bool isShow)
{
	if (isShow)
	{
		while (ShowCursor(TRUE) < 0) {} // カーソル表示
	}
	else
	{
		while (ShowCursor(FALSE) >= 0) {} // カーソル非表示
	}
}
