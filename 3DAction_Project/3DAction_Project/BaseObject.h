#pragma once

// ========================
// 基底オブジェクト
// ========================
#include "BaseDrawManager.h"

class BaseObject
{
protected:
	// 描画マネージャー
	BaseDrawManager* m_Draw = nullptr;

public:
	// コンストラクタ・デストラクタ
	BaseObject() = default;
	virtual ~BaseObject() = default;

	// 基本関数
	virtual void Init(BaseDrawManager* _drawManager) {
		m_Draw = _drawManager;
	}
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Uninit() = 0;
};

