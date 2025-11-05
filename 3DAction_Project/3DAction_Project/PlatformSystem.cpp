
// ========================================================
// ヘッダー
// ========================================================
// 必須ヘッダー
#include "PlatformSystem.h"

#if defined(DEBUG) || defined(_DEBUG)
// エラー用
#include <cassert> // エラー時にプロジェクトを停止させる

#endif


// ========================================================
// 静的変数
// ========================================================
#if defined(DEBUG) || defined(_DEBUG)
bool PlatformSystem::m_IsCreated = false; // 初期化

#endif


// ========================================================
// コンストラクタ
// ========================================================
PlatformSystem::PlatformSystem()
{
	m_Game = nullptr;

#if defined(DEBUG) || defined(_DEBUG)
	// デバッグ時のみ確認する
	if (m_IsCreated)
	{
		assert(false && "PlatformSystemクラスが複数生成されています");
	}
	m_IsCreated = true;
#endif
}


// ========================================================
// デストラクタ
// ========================================================
PlatformSystem::~PlatformSystem()
{
#if defined(DEBUG) || defined(_DEBUG)
	m_IsCreated = false;
#endif
}


// ========================================================
// 起動
// ========================================================
void PlatformSystem::Execute(BaseGame* game)
{
	m_Game = game;

	if (Init() && m_Game != nullptr) // 初期化
	{
		GameLoop(); // ゲームループ
	}
	Uninit(); // 終了処理
}