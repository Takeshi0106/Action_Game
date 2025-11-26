#pragma once

// =======================
// 基底ゲームクラス
// =======================
#include "BaseDrawManager.h"
#include "input.h"

class BaseGame
{
protected:
	BaseDrawManager* m_DrawManager = nullptr;
	Input* m_Input = nullptr;

public:
	// コンストラクタ・デストラクタ
	BaseGame() = default;
	virtual ~BaseGame() = default;

	// 共通初期化
	void Init(BaseDrawManager* drawManager, Input* input) {
		m_DrawManager = drawManager;
		m_Input = input;

		// 派生クラスの初期化
		DerivativeInit();
	}

	// 基本関数
	virtual void DerivativeInit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Uninit() = 0;
};

