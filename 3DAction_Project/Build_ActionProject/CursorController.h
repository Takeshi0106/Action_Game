#pragma once

// ==========================================
// 【クラス概要】
// カーソル制御を行う抽象化クラスです。
// ==========================================


// ==========================================
// 列挙型
// ==========================================
enum CursorMode
{
	// 通常モード
	CursorMode_Normal,
	// カーソル非表示モード
	CursorMode_Hidden,
	// カーソル固定・非表示モード
	CursorMode_Locked,
	// カーソル固定・非表示・中央に戻すモード
	CursorMode_LockedCenter,
};


// ==========================================
// クラス
// ==========================================
class CursorController
{
protected:
	// 現在のカーソルモード
	CursorMode m_CurrentMode = CursorMode_Normal;
	
	// モード切替時の処理
	virtual void CursorModeChanged() = 0;

public:
	// コンストラクタ・デストラクタ
	CursorController() = default;
	virtual ~CursorController() = default;

	// カーソルのモードを切り替える
	void SetCursorMode(CursorMode mode) {
		m_CurrentMode = mode;
		CursorModeChanged();
	}

	// カーソル位置を原点に戻す
	virtual void ResetCursorPosition() = 0;
};
