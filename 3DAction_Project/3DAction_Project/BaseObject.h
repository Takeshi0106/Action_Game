#pragma once

// ========================
// 基底オブジェクト
// ========================
#include "BaseDrawManager.h"
#include "Matrix4x4.h"
#include <string>

class BaseObject
{
protected:
	// 描画マネージャー
	BaseDrawManager* m_Draw = nullptr;

	// このオブジェクトが更新する定数バッファ名
	const std::string m_TransformCBName = "Transform";
	// SRT情報
	Matrix4x4 m_SRT;

	// 遅延初期化
	virtual void LateInit() = 0;

public:
	// コンストラクタ・デストラクタ
	BaseObject() = default;
	virtual ~BaseObject() = default;

	// 基本関数
	void Init(BaseDrawManager* _drawManager){
		m_Draw = _drawManager;
		// 派生初期化呼び出し
		LateInit();
	}

	// 描画
	virtual void Draw() = 0;
	virtual void Update() = 0;
	virtual void Uninit() = 0;
};

