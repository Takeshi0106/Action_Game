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
	BaseDrawManager* m_DrawManager = nullptr;
	Input* m_Input = nullptr;
	CursorController* m_CursorController = nullptr;

public:
	// コンストラクタ・デストラクタ
	BaseGame() = default;
	virtual ~BaseGame() = default;

	// 共通初期化
	void Init(BaseDrawManager* drawManager, Input* input, CursorController* cursorController) 
	{
		// 各情報初期化
		m_DrawManager = drawManager;
		m_Input = input;
		m_CursorController = cursorController;

		// 派生クラスの初期化
		DerivativeInit();
	}

	// 基本関数
	virtual void DerivativeInit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Uninit() = 0;
};

