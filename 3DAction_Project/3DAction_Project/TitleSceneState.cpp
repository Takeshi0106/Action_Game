
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
	// DCCカメラ初期化
	m_Camera.SetInput(m_Modules->input);
	if (!m_Camera.Init(m_Modules->drawManager)) {
		return true;
	}

	// プロジェクション行列（透視投影）
	float fov = 3.14159265f / 4.0f;
	float aspect = 1280.0f / 720.0f;
	float nearZ = 0.1f;
	float farZ = 100.0f;
	CameraProjInfo proj = { fov, aspect, nearZ, farZ };

	// カメラに設定
	m_Camera.SetProjection(proj);

	// 太陽光初期化
	if (!m_SunLight.Init(m_Modules->drawManager)) {
		return false;
	}

	// ナイトオブジェクト初期化
	m_Player.Init(m_Modules->drawManager);

	return true;
}


// ============================
// シーンの更新
// ============================
void TitleSceneState::DerivatIveUpdate(float _delta)
{
	// カメラ更新
	m_Camera.Update();
	// ライト更新
	m_SunLight.Update();
	// ナイトオブジェクト更新
	m_Player.Update();

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
	// カメラ情報をGPUに送る
	m_Camera.UpdateToGPU();
	// ライト情報をGPUに送る
	m_SunLight.UpdateToGPU();
	// ナイトオブジェクト描画
	m_Player.Draw();
}


// ============================
// シーンの終了処理
// ============================
void TitleSceneState::Uninit()
{
	// ナイトオブジェクト後処理
	m_Player.Uninit();
	// 太陽光後処理
	m_SunLight.Uninit();
	// カメラ後処理
	m_Camera.Uninit();
}
