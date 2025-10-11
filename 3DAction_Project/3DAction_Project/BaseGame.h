#pragma once

// =======================
// 基底ゲームクラス
// =======================
#include "BaseDrawManager.h"

class BaseGame
{
private:
	BaseDrawManager* BaseGameManager = nullptr;

public:
	// コンストラクタ・デストラクタ
	BaseGame() = default;
	virtual ~BaseGame() = default;

	// 基本関数
	virtual void Init(BaseDrawManager* drawManager) = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Uninit() = 0;
};

