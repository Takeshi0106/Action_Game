
// =============================
// ヘッダー
// =============================
// 必須ヘッダー
#include "TitleSceneState.h"



// ============================	
// シーンの初期化
// ============================
bool TitleSceneState::DerivativeInit()
{

	return true;
}


// ============================
// シーンの更新
// ============================
void TitleSceneState::DerivatIveUpdate(float _delta)
{

	// シーン遷移
	if (m_Modules->input->GetKeyTrigger(KeyCode_Enter)) {
		m_SceneEvent = SceneEventID::DEBUGSCENE;
	}
}


// ============================
// シーンの描画
// ============================
void TitleSceneState::Draw()
{

}


// ============================
// シーンの終了処理
// ============================
void TitleSceneState::Uninit()
{

}
