#pragma once

// 基底クラス
#include "BaseGame.h"

// クラス
class ActionGame :public BaseGame
{
private:
	// 描画マネージャー
	BaseDrawManager* m_DrawManager = nullptr;

public:
	ActionGame() = default;
	~ActionGame() = default;

	// 初期化
	void Init(BaseDrawManager* _drawManager) override;
	void Update() override;
	void Draw() override;
	void Uninit() override;
};

