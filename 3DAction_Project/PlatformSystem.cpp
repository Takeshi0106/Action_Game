#include "PlatformSystem.h"

// ========================================================
// プラットフォームごとに処理を変更できる基底クラス
// ========================================================

#if defined(DEBUG) || defined(_DEBUG)
bool PlatformSystem::m_IsCreated = false; // デバッグ時にコンパイル
#endif

// 起動
void PlatformSystem::Execute()
{
	if (Init()) // 初期化
	{
		GameLoop(); // ゲームループ
	}
	Uninit(); // 終了処理
}