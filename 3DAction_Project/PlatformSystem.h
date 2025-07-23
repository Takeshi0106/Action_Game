#pragma once
#include "NonCopyable.h"

#if defined(DEBUG) || defined(_DEBUG)
#include <cassert> // デバッグ時にヘッダーをコンパイル　(DebugLog)が出来たら変更
#endif

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

private:
	// -----------------------------------------------------------------
	// プラットフォームごとにオーバーライドする
	// -----------------------------------------------------------------
	virtual bool Init() = 0;	    // 初期化 (戻り値で初期化成功かを返す)
	virtual void Uninit() = 0;		// 後処理
	virtual void GameLoop() = 0;    // ゲームループ

public:
	explicit PlatformSystem() // コンストラクタ
	{
#if defined(DEBUG) || defined(_DEBUG)
		// デバッグ時のみ確認する
		if (m_IsCreated)
		{
			assert(false && "PlatformSystemクラスが複数生成されています");
		}
		m_IsCreated = true;
#endif
	}

	~PlatformSystem()// デストラクタ
	{
#if defined(DEBUG) || defined(_DEBUG)
		m_IsCreated = false;
#endif
	}

	void Execute(); // 起動する
};