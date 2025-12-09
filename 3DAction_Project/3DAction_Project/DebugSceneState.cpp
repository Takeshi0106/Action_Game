
// ===============================
// ヘッダー
// ===============================
// 必須ヘッダー
#include "DebugSceneState.h"
// カメラヘッダー
#include "DCCCamera3D.h"
// ライトヘッダー
#include "SunLight.h"
// ログ出力ヘッダー
#include "ReportMessage.h"

#if defined(DEBUG) || defined(_DEBUG)
// Imguiヘッダー
#include "imgui/imgui.h"
#endif


// ==============================
// シーンの初期化
// ==============================
bool DebugSceneState::DerivativeInit()
{
	// DCCカメラ初期化
	std::unique_ptr<DCCCamera3D> camera = std::make_unique<DCCCamera3D>();
	// カメラ初期化
	camera->SetInput(m_Modules->input);
	m_Camera = std::move(camera);

	// カメラ初期化
	if (!m_Camera->Init(m_Modules->drawManager)) {
		ErrorLog::OutputToConsole("DCCカメラの初期化に失敗");
		return false;
	}

	// プロジェクション行列（透視投影）
	float fov = 3.14159265f / 4.0f;
	float aspect = 1280.0f / 720.0f;
	float nearZ = 0.1f;
	float farZ = 100.0f;
	CameraProjInfo proj = { fov, aspect, nearZ, farZ };

	// カメラに設定
	m_Camera->SetProjection(proj);

	// 太陽光初期化
	m_Light = std::make_unique<SunLight>();
	// ライト初期化
	if (!m_Light->Init(m_Modules->drawManager)) {
		ErrorLog::OutputToConsole("太陽光の初期化に失敗");
		return false;
	}

	// オブジェクト初期化
	m_Square.Init(m_Modules->drawManager);
	m_Knight.Init(m_Modules->drawManager);
	m_Knight2.Init(m_Modules->drawManager);
	m_Knight2.SetPosition(Vector3(5.0f, 0.0f, 0.0f));

	return true;
}


// =============================
// シーンの更新
// =============================
void DebugSceneState::DerivatIveUpdate(float _deltaTime)
{
	// カメラ更新
	m_Camera->Update();
	// ライト更新
	m_Light->Update();

	// オブジェクト更新
	m_Square.Update();
	m_Knight.Update();
	m_Knight2.Update();

	// 当たったかチェック
	UpdateCollision();

	// シーン遷移イベント更新
	if (m_Modules->input->GetKeyTrigger(KeyCode_Enter)) {
		m_SceneEvent = SceneEventID::TITLESCENE;
	}

	DebugImgui();
}


// =============================
// シーンの描画
// =============================
void DebugSceneState::Draw()
{
	// カメラ情報をGPUに送る
	m_Camera->UpdateToGPU();
	// ライト情報をGPUに送る
	m_Light->UpdateToGPU();

	// オブジェクト描画
	m_Modules->drawManager->SetDepthStencilSetting(DepthStencilSetting::DepthEnableON_DepthWriteON);
	m_Knight.Draw();
	m_Knight2.Draw();

	// 透明物書き込み
	m_Modules->drawManager->SetDepthStencilSetting(DepthStencilSetting::DepthEnableON_DepthWriteOFF);
	m_Square.Draw();
}


// =============================
// シーンの終了処理
// =============================
void DebugSceneState::Uninit()
{
	// カメラ後処理
	m_Camera->Uninit();
	// ライト後処理
	m_Light->Uninit();

	// オブジェクト後処理
	m_Square.Uninit();
	m_Knight.Uninit();
	m_Knight2.Uninit();
}


// =============================
// 当たり判定更新
// =============================
void DebugSceneState::UpdateCollision()
{

}


#if defined(DEBUG) || defined(_DEBUG)
// =============================
// Imguiデバッグ
// =============================
void DebugSceneState::DebugImgui()
{
	Vector3 pos = m_Knight.GetSRT().position;
	float position[3] = {};

	ImGui::Begin("Knight1");

	ImGui::DragFloat3("Position", position, 1.0f, 10.0f);

	ImGui::End();

	// 反映
	m_Knight.SetPosition({ pos.x + position[0], pos.y + position[1], pos.z + position[2] });
}
#else
void DebugSceneState::DebugImgui() {}
#endif
