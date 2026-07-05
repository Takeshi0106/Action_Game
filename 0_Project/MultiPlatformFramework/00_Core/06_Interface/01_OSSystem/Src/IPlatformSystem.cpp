
// ========================================================
// ヘッダー
// ========================================================
// 必須ヘッダー
#include "IPlatformSystem.h"

// ========================================================
// 起動
// ========================================================
void IPlatformSystem::Execute(IGame* game, IDrawManager* drawManager)
{
	m_Game = game;
	m_DrawManager = drawManager;

	if (Init() && m_Game != nullptr) // 初期化
	{
		GameLoop(); // ゲームループ
	}
	Uninit(); // 終了処理
}
