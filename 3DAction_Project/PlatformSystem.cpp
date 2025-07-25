#include "PlatformSystem.h"

#if defined(DEBUG) || defined(_DEBUG)
#include <cassert> // デバッグ時にヘッダーをコンパイル　(DebugLog)が出来たら変更
#endif

// ========================================================
// プラットフォームごとに処理を変更できる基底クラス
// ========================================================

#if defined(DEBUG) || defined(_DEBUG)
bool PlatformSystem::m_IsCreated = false; // デバッグ時にコンパイル
#endif

// コンストラクタ
PlatformSystem::PlatformSystem()
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

// デストラクタ
PlatformSystem::~PlatformSystem()
{
#if defined(DEBUG) || defined(_DEBUG)
	m_IsCreated = false;
#endif
}

// 起動
void PlatformSystem::Execute()
{
	if (Init()) // 初期化
	{
		GameLoop(); // ゲームループ
	}
	Uninit(); // 終了処理
}