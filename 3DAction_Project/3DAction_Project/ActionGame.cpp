
// ==========================================
// ヘッダー
// ==========================================
// 必須
#include "ActionGame.h"
// ログ出力
#include "ReportMessage.h"
// タイマー
#include "Timer.h"

#if defined(DEBUG) || defined(_DEBUG)
// Imgui 使用ヘッダー
#include "imgui/imgui.h"
#endif


// =================================
// 初期化
// =================================
bool ActionGame::DerivativeInit()
{
	// シーンマネージャー初期化
	if (!m_SceneManager.Init(&m_Modules)) {
		ErrorLog::OutputToConsole("シーンマネージャーの初期化に失敗");
		return false;
	}

	// タイマー初期化・開始
	Timer::Init();
	Timer::Start();

	return true;
}


// ================================
// 更新
// ================================
bool ActionGame::Update()
{
	// タイマーデバッグ
	Timer::Debug_CheckUpdate();

	// 時間取得
	float time = Timer::GetDeltaTime();

	// シーン更新
	if (!m_SceneManager.Update(time)) {
		ErrorLog::OutputToConsole("シーンの更新に失敗");
		return false;
	}

	// Imguiを実行
	DebugImgui();

	// タイマー更新処理
	Timer::LastUpdate();

	return true;
}


// ================================
// 描画
// ================================
void ActionGame::Draw()
{
	// シーン描画
	m_SceneManager.Draw();
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


#if defined(DEBUG) || defined(_DEBUG)
// ================================
// Imgui使用
// ================================
void ActionGame::DebugImgui()
{
	// FPS出力
	ImGui::Begin("Debug Information");
	ImGuiIO& io = ImGui::GetIO();
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

	ImGui::End();
}
#else
// リリース時は何も実行しない
void ActionGame::DebugImgui() {}
#endif
