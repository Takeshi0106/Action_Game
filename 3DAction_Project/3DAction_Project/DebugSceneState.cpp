
// ===============================
// ヘッダー
// ===============================
// 必須ヘッダー
#include "DebugSceneState.h"
// カメラヘッダー
#include "DCCCamera3D.h"
// ライトヘッダー
#include "SunLight.h"
// コライダーヘッダー
#include "CollisionSystemModule.h"
// ログ出力ヘッダー
#include "ReportMessage.h"

#if defined(DEBUG) || defined(_DEBUG)
// Imguiヘッダー
#include "imgui/imgui.h"
// BOX描画
#include "BOX.h"
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
	m_Knight.SetPosition(Vector3(0.0f, 0.0f, 0.0f));
	m_Knight2.Init(m_Modules->drawManager);
	m_Knight2.SetPosition(Vector3(5.0f, 0.0f, 0.0f));

	// デバッグ初期化
	DebugInit();

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

	// デバッグ更新
	DebugUpdate();
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

	// デバッグ描画
	DebugDraw();
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
	// 当たり判定チェック
	if (m_CollisionSystem.CheckCollision(
		ColliderShapeType::AABB,
		(void*)&m_Knight.GetAABBCollider(),
		ColliderShapeType::AABB,
		(void*)&m_Knight2.GetAABBCollider()))
	{
		m_Knight.SetIsHit(true);
		m_Knight2.SetIsHit(true);
	}
}


#if defined(DEBUG) || defined(_DEBUG)
// =============================
// デバッグ初期化
// =============================
void DebugSceneState::DebugInit()
{
	// BOX描画初期化
	BOX::Init(m_Modules->drawManager);
}


// =============================
// デバッグ更新
// =============================
void DebugSceneState::DebugUpdate()
{
	Vector3 pos = m_Knight2.GetSRT().position;
	float position[3] = {};

	ImGui::Begin("Knight1");

	ImGui::DragFloat3("Position", position, 1.0f, 10.0f);

	ImGui::End();

	// 反映
	m_Knight2.SetPosition({ pos.x + position[0], pos.y + position[1], pos.z + position[2] });
}


// ----------------------------
// デバッグ描画
// ----------------------------
void DebugSceneState::DebugDraw()
{
	// アルファブレンド設定

	// AABB描画
	if (m_Knight.GetIsHit())
	{
		BOX::DrawAABB(m_Modules->drawManager, m_Knight.GetAABBCollider(), Color(1.0f, 0.0f, 0.0f, 0.3f));
	}
	else
	{
		BOX::DrawAABB(m_Modules->drawManager, m_Knight.GetAABBCollider(), Color(1.0f, 1.0f, 1.0f, 0.3f));
	}

	// AABB描画
	if (m_Knight2.GetIsHit())
	{
		BOX::DrawAABB(m_Modules->drawManager, m_Knight2.GetAABBCollider(), Color(1.0f, 0.0f, 0.0f, 0.3f));
	}
	else
	{
		BOX::DrawAABB(m_Modules->drawManager, m_Knight2.GetAABBCollider(), Color(1.0f, 1.0f, 1.0f, 0.3f));
	}
}

#else
// リリース時のダミー関数
void DebugSceneState::DebugInit() {}
void DebugSceneState::DebugUpdate() {}
void DebugSceneState::DebugDraw() {}

#endif
