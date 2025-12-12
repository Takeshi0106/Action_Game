
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
// 時間
#include "Timer.h"
// 文字列
#include <string>
#endif


// ==============================
// シーンの初期化
// ==============================
bool DebugSceneState::DerivativeInit()
{
	Timer::Init();
	Timer::Start();

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
	for (int i = 0; i < m_KnightCount; i++)
	{
		m_Knight[i].Init(m_Modules->drawManager);
		m_Knight[i].SetPosition(Vector3(static_cast<float>(i * 10.0f), 0.0f, 0.0f));
		m_Knight[i].Update();
	}

#if defined(DEBUG) || defined(_DEBUG)
	// １フレームの時間
	float time = Timer::GetDeltaTime();
#endif

	// AABB登録
	std::vector<uint32_t> nodeIDs;
	nodeIDs.reserve(m_KnightCount + 1);
	m_AABBTree.Reserve(m_KnightCount + 1);

	for (int i = 0; i < m_KnightCount; i++)
	{
		uint32_t knightID = m_AABBTree.AddNode(m_Knight[i].GetAABBCollider(), m_Knight[i].GetObjectInfo());
		nodeIDs.push_back(knightID);
	}

	// ツリー再ビルド
	// m_AABBTree.RebuildTree();

#if defined(DEBUG) || defined(_DEBUG)
	// 時間出力
	float outputTime = Timer::GetDeltaTime() - time;
	DebugLog::OutputToConsole((std::to_string(outputTime) + "秒 : AABB登録時間").c_str());
#endif

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
	for (int i = 0; i < m_KnightCount; i++)
	{
		m_Knight[i].Update();
	}

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
	for (int i = 0; i < m_KnightCount; i++)
	{
		m_Knight[i].Draw();
	}

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
	for (int i = 0; i < m_KnightCount; i++)
	{
		m_Knight[i].Uninit();
	}
}


// =============================
// 当たり判定更新
// =============================
void DebugSceneState::UpdateCollision()
{
#if defined(DEBUG) || defined(_DEBUG)
	// １フレームの時間
	float time = Timer::GetDeltaTime();
#endif

	// 衝突候補取得
	std::vector<ObjectInfo> results;
	results.reserve(m_KnightCount + 1);

	for (int i = 0; i < m_KnightCount; i++)
	{
		m_AABBTree.Query(m_Knight[i].GetAABBCollider(), results);
		if (results.size() > 1)
		{
			m_Knight[i].SetIsHit(true);
		}
		results.clear();
		results.reserve(m_KnightCount + 1);
	}

#if defined(DEBUG) || defined(_DEBUG)
	// 時間出力
	float outputTime = Timer::GetDeltaTime() - time;
	ImGui::Begin("CollisionTime");
	ImGui::Text((std::to_string(outputTime) + "秒 : 当たり判定更新時間").c_str());
	ImGui::End();
#endif
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
	Vector3 pos = m_Knight[0].GetSRT().position;
	Quaternion rot = m_Knight[0].GetSRT().rotation;

	float position[3] = {};
	float rotation[3] = {};

	ImGui::Begin("Knight1");

	ImGui::DragFloat3("Position", position, 1.0f, 10.0f);
	ImGui::DragFloat3("Rotation", rotation, 1.0f, 360.0f);

	ImGui::End();

	// 反映
	m_Knight[0].SetPosition({pos.x + position[0], pos.y + position[1], pos.z + position[2]});
	// オイラー角をクォータニオンに変換
	Quaternion newRot = Quaternion::CreateQuaternionFromEuler(rotation[0], rotation[1], rotation[2]);
	m_Knight[0].SetRotation(rot * newRot);
}


// ----------------------------
// デバッグ描画
// ----------------------------
void DebugSceneState::DebugDraw()
{
	for (int i = 0 ; i < m_KnightCount; i++)
	{
		// AABB描画
		if (m_Knight[i].GetIsHit())
		{
			BOX::DrawAABB(m_Modules->drawManager, m_Knight[i].GetAABBCollider(), Color(1.0f, 0.0f, 0.0f, 0.3f));
		}
		else
		{
			BOX::DrawAABB(m_Modules->drawManager, m_Knight[i].GetAABBCollider(), Color(1.0f, 1.0f, 1.0f, 0.3f));
		}
	}
}

#else
// リリース時のダミー関数
void DebugSceneState::DebugInit() {}
void DebugSceneState::DebugUpdate() {}
void DebugSceneState::DebugDraw() {}

#endif
