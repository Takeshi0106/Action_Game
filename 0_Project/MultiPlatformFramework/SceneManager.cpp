
// ========================================
// ヘッダー
// ========================================
// 必須ヘッダー
#include "SceneManager.h"
// シーンヘッダー
#include "DebugSceneState.h"
#include "TitleSceneState.h"
// ログ出力ヘッダー
#include "ReportMessage.h"


// ========================================
// シーンの初期化
// ========================================
bool SceneManager::Init(GameModules* modules)
{
	// マネージャー保存
	m_Modules = modules;

#if defined(DEBUG) || defined(_DEBUG)
	// 最初のシーンを設定する
	m_CurrentSceneState = std::make_unique<DebugSceneState>();
#else
	// 最初のシーンを設定する
	m_CurrentSceneState = std::make_unique<TitleSceneState>();
#endif

	// シーン初期化
	if (!m_CurrentSceneState->Init(m_Modules)) {
		ErrorLog::OutputToConsole(u8"シーンの初期化に失敗");
		return false;
	}

	return true;
}


// ========================================
// シーンの更新
// ========================================
bool SceneManager::Update(float time)
{
	// シーン更新
	m_CurrentSceneState->Update(time);

	// シーンイベント取得
	SceneEventID event = m_CurrentSceneState->GetSceneEvent();

	// シーン切り替え
	if (event != SceneEventID::NONE)
	{
		// ゲーム終了イベントならfalseを返す
		if (event == SceneEventID::STOP_GAME)
		{
			return false;
		}

		return ChangeScene(event);
	}

	return true;
}


// ========================================
// シーンの描画
// ========================================
void SceneManager::Draw()
{
	// シーン描画
	m_CurrentSceneState->Draw();
}


// ========================================
// シーンの終了処理
// ========================================
void SceneManager::Uninit()
{
	// null チェック
	if (m_CurrentSceneState != nullptr)
	{
		// シーンの終了処理
		m_CurrentSceneState->Uninit();
		m_CurrentSceneState.reset();
	}
}


// ========================================
// シーンの切り替え
// ========================================
bool SceneManager::ChangeScene(SceneEventID event)
{
	// 現在のシーンの終了処理
	m_CurrentSceneState->Uninit();
	m_CurrentSceneState.reset();

	// 新しいシーンを設定する
	switch (event)
	{
		// デバッグシーン
	case SceneEventID::DEBUGSCENE:
		m_CurrentSceneState = std::make_unique<DebugSceneState>();
		break;
		// タイトルシーン
	case SceneEventID::TITLESCENE:
		m_CurrentSceneState = std::make_unique<TitleSceneState>();
		break;


		// エラーログ
	default:
		ErrorLog::OutputToConsole(u8"シーンイベントが不正です" + String::to_u8string((uint64_t)event));
		break;
	}

	// 新しいシーンの初期化
	if (!m_CurrentSceneState->Init(m_Modules)) {
		ErrorLog::OutputToConsole(u8"シーンの初期化に失敗");
		return false;
	}

	return true;
}
