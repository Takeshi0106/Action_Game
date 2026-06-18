#pragma once

// ===================================
// 【クラス概要】
// オブジェクトシステムクラス
// ===================================


// ===================================
// ヘッダー
// ===================================
#include <vector>
#include "Vector3.h"
#include "IDrawManager.h"



// ===================================
// クラス
// ===================================
class ObjectSystem
{
private:

public:
	// --------------------------------
	// コンストラクタ・デストラクタ
	// --------------------------------
	ObjectSystem() = default;
	virtual ~ObjectSystem() = default;


	// --------------------------------
	// メンバー関数
	// --------------------------------
	virtual bool Init(IDrawManager* _draw) = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Draw() = 0;
	virtual void Uninit() = 0;
};

