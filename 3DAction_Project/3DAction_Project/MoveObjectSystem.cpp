
// =====================================
// ヘッダー
// =====================================
#include "MoveObjectSystem.h"
#include "ReportMessage.h"



// =====================================
// 定数
// =====================================
namespace MoveObject_AABB {
	// コライダーオフセット
	constexpr Vector3 CENTER_OFFSET = { 0.0f,1.5f,0.0f };
	// ヒットボックススケール
	constexpr Vector3 HITBOX_SCALE = { 3.5f, 3.0f, 2.8f };

	// プレイヤーのコライダー設定
	constexpr ColliderPresetConfig COLLIDER_CONFIG = {
		CENTER_OFFSET,
		HITBOX_SCALE,
		ColliderShapeType::AABB,
		ObjectInfo{0, ObjectTag::NOTAG} };
}


// =====================================
// 初期化
// =====================================
bool MoveObjectSystem::Init(BaseDrawManager* _draw)
{
	// 描画マネージャー保存
	m_Draw = _draw;

	// モデルのロード
	m_Draw->LoadModel(m_ModelName.c_str(), "Character");

	// SRT初期化
	SRT srt = {
		Vector3(),
		Quaternion(),
		kObjectSize };

	// 定数バッファ作成
	m_Draw->CreateConstantBuffer(
		m_TransformCBName.c_str(),
		&srt,
		sizeof(srt),
		BufferUsage::Dynamic,
		CPUAccess::Write);

	// 配列メモリ確保
	m_Positions.reserve(kObjectCount);
	m_Rotations.reserve(kObjectCount);
	m_AABBColliders.reserve(kObjectCount);
	m_FatAABBColliders.reserve(kObjectCount);
	m_AABBHandle.reserve(kObjectCount);
	m_Correction.resize(kObjectCount, Vector3(0.0f, 0.0f, 0.0f));

	float radius = 50.0f;  // 円の半径
	Vector3 center(0.0f, -10.0f, 10.0f);

	// オブジェクト数分ループ
	for (int i = 0; i < kObjectCount; i++)
	{
		// 位置初期化    
		float angle = (float)i / (float)kObjectCount * 6.283185307f; // 2π

		// XZ 平面に円状配置
		float x = center.x + std::cos(angle) * radius;
		float z = center.z + std::sin(angle) * radius;
		float y = 0.0f;

		m_Positions.push_back(Vector3(x, y, z));

		Vector3 dir = (center - m_Positions[i]).Normalize();
		dir.y = 0.0f;
		Quaternion rot = Quaternion::LookRotation(dir, Vector3(0, 1, 0));
		rot = Quaternion();
		m_Rotations.push_back(rot);

		srt.position = m_Positions[i];
		srt.rotation = m_Rotations[i];

		srt.UpdateWorldMatrix();

		// AABBコライダー初期化
		AABBCollider aabbCollider = CreateAABB(
			srt,
			MoveObject_AABB::CENTER_OFFSET,
			MoveObject_AABB::HITBOX_SCALE);

		m_AABBColliders.push_back(aabbCollider);

		// ファットAABBコライダー初期化
		FatAABBCollider fatAabbCollider = CreateFatAABB(
			m_AABBColliders[i],
			Vector3(kMoveVelocity,kMoveVelocity,kMoveVelocity),
			3.0f);

		m_FatAABBColliders.push_back(fatAabbCollider);
	}

	return true;
}


// =====================================
// 更新
// =====================================
void MoveObjectSystem::Update(float deltaTime)
{
	for (int i = 0; i < kObjectCount; i++)
	{
		// -------------------------
		// 移動
		// -------------------------
		Vector3 toTarget = m_TargetPos - m_Positions[i];
		float dist = toTarget.Length();

		if (dist > 0.0001f)
		{
			Vector3 dir = toTarget.Normalize();

			// 位置更新
			m_Positions[i] += dir * kMoveVelocity;

			// -------------------------
			// 回転更新（移動方向へ振り向く）
			// -------------------------
			Quaternion targetRot = Quaternion::LookRotation(dir, Vector3(0, 1, 0));

			// Slerpで滑らかに回す
			m_Rotations[i] = m_Rotations[i].Slerp(
				targetRot,
				kRotateSpeed * deltaTime);
		}

		// SRT情報更新
		SRT srt = {
			m_Positions[i],
			m_Rotations[i],
			kObjectSize };
		srt.UpdateWorldMatrix();

		// AABBコライダー更新
		m_AABBColliders[i] = CreateAABB(
			srt,
			MoveObject_AABB::CENTER_OFFSET,
			MoveObject_AABB::HITBOX_SCALE);

		if (!IsAABBInside(m_AABBColliders[i], m_FatAABBColliders[i]))
		{
			// ファットAABBコライダー更新
			m_FatAABBColliders[i] = CreateFatAABB(
				m_AABBColliders[i],
				Vector3(kMoveVelocity, kMoveVelocity, kMoveVelocity),
				3.0f);
		}
	}
}


// =====================================
// 描画
// =====================================
void MoveObjectSystem::Draw()
{
	for (int i = 0; i < kObjectCount; i++)
	{
		// SRT情報更新
		SRT srt = {
			m_Positions[i],
			Quaternion(),
			kObjectSize };

		Matrix4x4 world = srt.UpdateWorldMatrix().toGPU();

		// 定数バッファ更新
		m_Draw->UpdateShaderConstants(
			m_TransformCBName.c_str(),
			&world,
			sizeof(world));

		// モデル描画
		m_Draw->ModelDraw(
			m_VSName.c_str(),
			m_PSName.c_str(),
			m_ModelName.c_str());
	}
}


// =====================================
// 後処理
// =====================================
void MoveObjectSystem::Uninit()
{

}


// =====================================
// 当たり判定処理
// =====================================
Vector3 MoveObjectSystem::ComputeMTV(uint32_t _selfInd, uint32_t _partnerInd)
{
	AABBCollider& a = m_AABBColliders[_selfInd];
	AABBCollider& b = m_AABBColliders[_partnerInd];

	float dx1 = b.max.x - a.min.x;
	float dx2 = a.max.x - b.min.x;
	float overlapX = (dx1 < dx2) ? dx1 : -dx2;

	float dy1 = b.max.y - a.min.y;
	float dy2 = a.max.y - b.min.y;
	float overlapY = (dy1 < dy2) ? dy1 : -dy2;

	float dz1 = b.max.z - a.min.z;
	float dz2 = a.max.z - b.min.z;
	float overlapZ = (dz1 < dz2) ? dz1 : -dz2;

	float ax = std::fabs(overlapX);
	float ay = std::fabs(overlapY);
	float az = std::fabs(overlapZ);

	// 最小軸を返す
	if (ax <= ay && ax <= az) return Vector3(overlapX, 0, 0);
	if (ay <= ax && ay <= az) return Vector3(0, overlapY, 0);
	return Vector3(0, 0, overlapZ);
}


// =====================================
// 押し戻し更新
// =====================================
void MoveObjectSystem::UpdateCorrection()
{
	// 押し戻し処理
	for (int i = 0; i < kObjectCount; i++)
	{
		// 位置補正
		m_Positions[i] += m_Correction[i];
		// 補正値リセット
		m_Correction[i] = Vector3(0.0f, 0.0f, 0.0f);

		// 押し戻し後
		m_FatAABBColliders[i] = CreateFatAABB(
			m_AABBColliders[i],
			Vector3(kMoveVelocity, kMoveVelocity, kMoveVelocity),
			3.0f);
	}
}
