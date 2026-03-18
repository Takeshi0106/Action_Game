

// =====================================
// ヘッダー
// =====================================
#include "FadeManager.h"


// =====================================
// 初期化
// =====================================
bool FadeManager::Init(BaseDrawManager* _draw)
{
	// 描画用マネージャーセット
	m_DrawManager = _draw;

	// ノーマルフェードを設定

	return true;
}


// =====================================
// 更新
// =====================================
bool FadeManager::FadeUpdate(float deltaTime)
{
	// 経過時間更新
	m_FadeElapsedTime += deltaTime;

	// フェード時間超過チェック
	if (m_FadeElapsedTime >= m_FadeTime)
	{
		// フェード完了
		m_FadeElapsedTime = m_FadeTime;

		// フェード状態更新
		m_FadeState->Update(m_FadeElapsedTime / m_FadeTime);
		// 初期化
		m_FadeElapsedTime = 0.0f;

		return true;
	}

	// フェード

	return false;
}


// =====================================
// 終了処理
// =====================================
void FadeManager::Uninit()
{

}


// =====================================
// フェード変更
// =====================================
void FadeManager::ChangeFadeState(FadeID _fade)
{
	//switch (_fade)
	//{

	//}

}
