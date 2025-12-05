
// =============================
// ヘッダー
// =============================
// 必須ヘッダー
#include "TitleScene.h"



// ============================	
// シーンの初期化
// ============================
bool TitleScene::DerivativeInit()
{

	return true;
}


// ============================
// シーンの更新
// ============================
void TitleScene::DerivatIveUpdate(float _delta)
{
	if (m_Modules->input->GetKeyPress(KeyCode_Enter)) {
		m_SceneEvent = SceneEventID::DEBUGSCENE;
	}
}


// ============================
// シーンの描画
// ============================
void TitleScene::Draw()
{

}


// ============================
// シーンの終了処理
// ============================
void TitleScene::Uninit()
{

}
