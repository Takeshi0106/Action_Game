#pragma once
#include "NonCopyable.h"

// =============================================================================================
// プラットフォームごとで継承して、初期化・後処理をポリモーフィズムする基底クラス
// このクラスを継承したクラスは１つしか生成できないことを保証する
// =============================================================================================

class PlatformSystem : public NonCopyable
{
public:
#if defined(DEBUG) || defined(_DEBUG)
	static bool m_IsCreated; // １つしかないことを保証する
#endif

protected:
	// -----------------------------------------------------------------
	// プラットフォームごとにオーバーライドする
	// -----------------------------------------------------------------
	virtual bool Init() = 0;	    // 初期化 (戻り値で初期化成功かを返す)
	virtual void Uninit() = 0;		// 後処理
	virtual void GameLoop() = 0;    // ゲームループ
	virtual void GameInit() = 0;    // ゲーム初期化
	virtual void GameMain() = 0;    // ゲームメイン
	virtual void GameUninit() = 0;  // ゲーム後処理

public:
	explicit PlatformSystem(); // コンストラクタ
	~PlatformSystem();         // デストラクタ

	void Execute(); // 起動する
};