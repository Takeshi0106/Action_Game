
// =====================================
// ヘッダー
// =====================================
#include "MoveObjectSystem.h"



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
	m_AABBColliders.reserve(kObjectCount);
	m_FatAABBColliders.reserve(kObjectCount);

	// オブジェクト数分ループ
	for (int i = 0; i < kObjectCount; i++)
	{
		// 位置初期化
		m_Positions.push_back(Vector3(i * 5.0f, 0.0f, 0.0f));

		srt.position = m_Positions[i];

		srt.UpdateWorldMatrix();

		// AABBコライダー初期化
		AABBCollider aabbCollider = CreateAABB(
			srt,
			MoveObject_AABB::CENTER_OFFSET,
			MoveObject_AABB::HITBOX_SCALE);

		m_AABBColliders.push_back(aabbCollider);

		// ファットAABBコライダー初期化
		AABBCollider fatAabbCollider = CreateAABB(
			srt,
			MoveObject_AABB::CENTER_OFFSET,
			MoveObject_AABB::HITBOX_SCALE);

		m_FatAABBColliders.push_back(fatAabbCollider);
	}

	return true;
}


// =====================================
// 更新
// =====================================
void MoveObjectSystem::Update(float deltaTime)
{

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
void MoveObjectSystem::OnCollision()
{

}
