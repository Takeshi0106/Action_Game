#pragma once

// ========================================
// 【クラス概要】
// APIシステムのインターフェイス(基底クラス)
// ========================================
// ヘッダー
#include "IWindowHandle.h"

// クラス
class IAPISystem
{
public:
	// 初期化・後処理
	virtual bool Init(uint32_t _width, uint32_t _height, IWindowHandle& _handle) = 0;
	virtual void Uninit() = 0;

	// ゲッター関数
	virtual const IDrawCreate* GetResourceFactory() = 0;
};
