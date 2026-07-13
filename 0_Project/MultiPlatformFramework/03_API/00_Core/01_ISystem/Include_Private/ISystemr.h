#pragma once

// ========================================
// 【クラス概要】
// APIシステムのインターフェイス(基底クラス)
// ========================================

class IAPISystem
{
protected:


public:
	// 初期化
	virtual bool Init(uint16_t _width, uint16_t _height ) = 0;
	// 後処理
	virtual void Uninit() = 0;

	// ゲッター関数
	virtual const IDrawCreate* GetResourceFactory() = 0;
};
