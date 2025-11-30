
// ==========================================
// ヘッダー
// ==========================================
// 必須
#include "ActionGame.h"
// 文字列
#include <string>
// ログ出力
#include "ReportMessage.h"
// タイマー
#include "Timer.h"


// =================================
// 初期化
// =================================
bool ActionGame::DerivativeInit()
{
	// シーンマネージャー初期化
	m_SceneManager.Init(m_DrawManager, m_Input, m_CursorController);

	// タイマー初期化・開始
	Timer::Init();
	Timer::Start();

	return true;
}


// ================================
// 更新
// ================================
void ActionGame::Update()
{
	// タイマーデバッグ
	Timer::Debug_CheckUpdate();

	// シーン更新
	m_SceneManager.Update(Timer::GetDeltaTime());

#if defined(DEBUG) || defined(_DEBUG)
	// 時間を取得
	m_FPSTime += Timer::GetDeltaTime();
	m_FPSCount++;

	if (m_FPSTime > 1.0f)
	{
		// ログ出力
		WarningLog::OutputToConsole(std::string("1フレームのFPS " + std::to_string(m_FPSCount)).c_str());

		// リセット
		m_FPSTime = 0.0f;
		m_FPSCount = 0;
	}
#endif

	// タイマー更新処理
	Timer::LastUpdate();
}


// ================================
// 描画
// ================================
void ActionGame::Draw()
{
	// 描画前
	m_DrawManager->BegingDraw();

	// シーン描画
	m_SceneManager.Draw();

	// 描画後
	m_DrawManager->EndDraw();
}


// ================================
// 後処理
// ================================
void ActionGame::Uninit()
{
	// シーンマネージャー後処理
	m_SceneManager.Uninit();
	// タイマー削除
	Timer::Label::ClearTimers();
}
