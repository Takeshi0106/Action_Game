
// ========================================
// ヘッダー
// ========================================
// 必須ヘッダー
#include "SceneManager.h"
// シーンヘッダー
#include "DebugScene.h"


// ========================================
// シーンの初期化
// ========================================
void SceneManager::Init(BaseDrawManager* _drawManager, Input* _input, CursorController* _cursor)
{
	// マネージャー保存
	m_DrawManager = _drawManager;
	m_Input = _input;
	m_CursorController = _cursor;

	// 最初のシーンを設定する
	m_CurrentSceneState = std::make_unique<DebugScene>();
	// シーン初期化
	m_CurrentSceneState->Init(m_DrawManager, m_Input, m_CursorController);
}


// ========================================
// シーンの更新
// ========================================
void SceneManager::Update(float time)
{
	// シーン更新
	m_CurrentSceneState->Update(time);

	// シーンイベント取得
	SceneEvent event = m_CurrentSceneState->GetSceneEvent();

	if (!event == SceneEvent::SCENE_EVENT_NONE) 
	{
		ChangeScene(event);
	}
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
	// シーンの終了処理
	m_CurrentSceneState->Uninit();
	m_CurrentSceneState.reset();
}


// ========================================
// シーンの切り替え
// ========================================
void SceneManager::ChangeScene(SceneEvent event)
{
	// 現在のシーンの終了処理
	m_CurrentSceneState->Uninit();
	m_CurrentSceneState.reset();

	// 新しいシーンを設定する
	switch (event)
	{
	case SCENE_EVENT_DEBUG_SCENE:
		m_CurrentSceneState = std::make_unique<DebugScene>();
		break;
	}

	// 新しいシーンの初期化
	m_CurrentSceneState->Init(m_DrawManager, m_Input, m_CursorController);
}
