#pragma once

// =======================
// ヘッダー
// =======================
// 描画マネージャー
#include "BaseDrawManager.h"
// 入力情報
#include "input.h"
// カーソル制御
#include "CursorController.h"


// =======================
// クラス
// =======================
class BaseGame
{
protected:
	// マネージャー
	BaseDrawManager* m_DrawManager = nullptr;
	Input* m_Input = nullptr;
	CursorController* m_CursorController = nullptr;

	// 派生初期化
	virtual bool DerivativeInit() = 0;

public:
	// コンストラクタ・デストラクタ
	BaseGame() = default;
	virtual ~BaseGame() = default;

	// 共通初期化
	bool Init(BaseDrawManager* drawManager, Input* input, CursorController* cursorController) 
	{
		// 各情報初期化
		m_DrawManager = drawManager;
		m_Input = input;
		m_CursorController = cursorController;

		// 派生クラスの初期化
		return DerivativeInit();
	}

	// 基本関数
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Uninit() = 0;
};

