
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
	if (m_Modules->input->GetKeyPress(KeyCode_Enter)) {
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
