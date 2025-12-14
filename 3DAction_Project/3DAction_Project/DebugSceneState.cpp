
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
#if defined(DEBUG) || defined(_DEBUG)
	Timer::Init();
	Timer::Start();
#endif

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

	// 初期化
	m_MoveObjectSystem.Init(m_Modules->drawManager);

#if defined(DEBUG) || defined(_DEBUG)
	// １フレームの時間
	float time = Timer::GetDeltaTime();
#endif

	std::vector<FatAABBCollider>& col = m_MoveObjectSystem.GetFatAABBColliders();
	std::vector<AABBTreeHandle>& handles = m_MoveObjectSystem.GetAABBHandle();

	for (uint32_t i = 0; i < col.size(); i++)
	{
		AABBTreeHandle handle = m_AABBTree.AddNode(col[i].aabb, ObjectInfo(i, ObjectTag::NOTAG));
		handles.push_back(handle);
	}

	// ツリー再ビルド
	m_AABBTree.RebuildTree();

	// 深度情報取得
	TreeBalance  balance = m_AABBTree.CalculateBalance();

#if defined(DEBUG) || defined(_DEBUG)
	// 時間出力
	float outputTime = Timer::GetDeltaTime() - time;
	DebugLog::OutputToConsole((std::to_string(outputTime) + "秒 : AABB登録時間").c_str());
	DebugLog::OutputToConsole((std::to_string(balance.averageDepth) + " : 平均深度").c_str());
	DebugLog::OutputToConsole((std::to_string(balance.maxDepth) + " : 最大深度").c_str());
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
	m_MoveObjectSystem.Update(_deltaTime);

	// 
	UpdateColliderCheck();

	// 当たったかチェック
	UpdateCollision();

	// シーン遷移イベント更新
	if (m_Modules->input->GetKeyTrigger(KeyCode_Enter)) {
		m_SceneEvent = SceneEventID::TITLESCENE;
	}

	if (m_Modules->input->GetKeyTrigger(KeyCode_Escape)) {
		m_SceneEvent = SceneEventID::STOP_GAME;
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

#if defined(DEBUG) || defined(_DEBUG)
	// １フレームの時間
	float time = Timer::GetDeltaTime();
#endif
	// オブジェクト描画
	m_Modules->drawManager->SetDepthStencilSetting(DepthStencilSetting::DepthEnableON_DepthWriteON);
	m_MoveObjectSystem.Draw();

#if defined(DEBUG) || defined(_DEBUG)
	// 時間出力
	float outputTime = Timer::GetDeltaTime() - time;
	ImGui::Begin("DrawTime");
	ImGui::Text((std::to_string(outputTime) + "秒 : 描画時間").c_str());
	ImGui::End();
#endif

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
	m_MoveObjectSystem.Uninit();
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
	std::vector<FatAABBCollider>& fatcol = m_MoveObjectSystem.GetFatAABBColliders();
	std::vector<AABBCollider>& col = m_MoveObjectSystem.GetAABBColliders();
	std::vector<Vector3>& correction = m_MoveObjectSystem.GetCorrection();

	// 接触候補配列
	std::vector<ObjectInfo> results;
	results.reserve(fatcol.size());

	// 当たり判定
	for (uint32_t i = 0; i < fatcol.size(); i++)
	{
		results.clear();
		// ツリー検索
		m_AABBTree.Query(fatcol[i].aabb, results);

		// 接触候補と当たり判定
		for (const auto& result : results)
		{
			// 自分自身は除外
			if (result.objectID <= i) { continue; }

			// 厳密な当たり判定
			if (m_CollisionSystem.CheckCollision(
				ColliderShapeType::AABB,
				&col[i],
				ColliderShapeType::AABB,
				&col[result.objectID]))
			{

				Vector3 mtv = m_MoveObjectSystem.ComputeMTV(i, result.objectID);

				correction[i] += mtv * 0.5f;
				correction[result.objectID] += -mtv * 0.5f;
			}
		}
	}

	// 押し戻し更新
	m_MoveObjectSystem.UpdateCorrection();

#if defined(DEBUG) || defined(_DEBUG)
	// 時間出力
	float outputTime = Timer::GetDeltaTime() - time;
	ImGui::Begin("CollisionTime");
	ImGui::Text((std::to_string(outputTime) + "秒 : 当たり判定更新時間").c_str());
	ImGui::End();

	// 深度情報取得
	TreeBalance balance = m_AABBTree.CalculateBalance();
	ImGui::Begin("AABBTreeBalance");
	ImGui::Text((std::to_string(balance.averageDepth) + " : 平均深度").c_str());
	ImGui::Text((std::to_string(balance.maxDepth) + " : 最大深度").c_str());
	ImGui::End();
#endif
}


// =============================
// コライダー更新チェック
// =============================
void DebugSceneState::UpdateColliderCheck()
{
#if defined(DEBUG) || defined(_DEBUG)
	// １フレームの時間
	float time = Timer::GetDeltaTime();
#endif

	std::vector<FatAABBCollider>& fatcol = m_MoveObjectSystem.GetFatAABBColliders();
	std::vector<AABBTreeHandle>& handles = m_MoveObjectSystem.GetAABBHandle();

	for (uint32_t i = 0; i < fatcol.size(); i++)
	{
		if (fatcol[i].isUpdated)
		{
			m_AABBTree.Remove(handles[i]);
			handles[i] = m_AABBTree.AddNode(fatcol[i].aabb, ObjectInfo(i, ObjectTag::NOTAG));
			fatcol[i].isUpdated = false;
		}
	}

#if defined(DEBUG) || defined(_DEBUG)
	// 時間出力
	float outputTime = Timer::GetDeltaTime() - time;
	ImGui::Begin("ChaeckCollider");
	ImGui::Text((std::to_string(outputTime) + "秒 : 更新時間").c_str());
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
	Vector3 targetPos = m_MoveObjectSystem.GetTargetPos();
	float pos[3] = { targetPos.x, targetPos.y, targetPos.z };

	ImGui::Begin("TargetPos");
	ImGui::DragFloat3("Position", pos, 0.1f, -100.0f, 100.0f);
	ImGui::End();

	// 反映
	m_MoveObjectSystem.SetTargetPos({ pos[0], pos[1], pos[2] });

	//Vector3 pos = m_Knight[0].GetSRT().position;
	//Quaternion rot = m_Knight[0].GetSRT().rotation;

	//float position[3] = {};
	//float rotation[3] = {};

	//ImGui::Begin("Knight1");

	//ImGui::DragFloat3("Position", position, 1.0f, 10.0f);
	//ImGui::DragFloat3("Rotation", rotation, 1.0f, 360.0f);

	//ImGui::End();

	//// 反映
	//m_Knight[0].SetPosition({pos.x + position[0], pos.y + position[1], pos.z + position[2]});
	//// オイラー角をクォータニオンに変換
	//Quaternion newRot = Quaternion::CreateQuaternionFromEuler(rotation[0], rotation[1], rotation[2]);
	//m_Knight[0].SetRotation(rot * newRot);
}


// ----------------------------
// デバッグ描画
// ----------------------------
void DebugSceneState::DebugDraw()
{
	Vector3 targetPos = m_MoveObjectSystem.GetTargetPos();

	SRT srt = {
		targetPos,
		Quaternion(),
		{1.0f,1.0f,1.0f} };

	AABBCollider aabb = CreateAABB(
		srt,
		Vector3(0.0f, 0.0f, 0.0f),
		Vector3(1.0f, 1.0f, 1.0f));

	BOX::DrawAABB(
		m_Modules->drawManager,
		aabb,
		Color(0.0f, 0.5f, 0.0f, 1.0f));
}

#else
// リリース時のダミー関数
void DebugSceneState::DebugInit() {}
void DebugSceneState::DebugUpdate() {}
void DebugSceneState::DebugDraw() {}

#endif
