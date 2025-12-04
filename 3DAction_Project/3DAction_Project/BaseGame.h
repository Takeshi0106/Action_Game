#pragma once

// =======================
// ヘッダー
// =======================
// モジュール
#include "GameModule.h"


// =======================
// クラス
// =======================
class BaseGame
{
protected:
	// マネージャー
	GameModules m_Modules;

	// 派生初期化
	virtual bool DerivativeInit() = 0;

public:
	// コンストラクタ・デストラクタ
	BaseGame() = default;
	virtual ~BaseGame() = default;

	// 共通初期化
	bool Init(GameModules& modules) 
	{
		// 各情報初期化
		m_Modules = modules;

		// 派生クラスの初期化
		return DerivativeInit();
	}

	// 基本関数
	virtual bool Update() = 0;
	virtual void Draw() = 0;
	virtual void Uninit() = 0;
};

