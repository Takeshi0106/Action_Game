
// ============================
// ヘッダー
// ============================
// 必須ヘッダー
#include "Knight.h"
// 計算ヘッダー
#include "Vector3.h"
// コライダー設定
#include "ColliderConfig.h"


// ============================
// 定数
// ============================
namespace Debug_AABB {
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



// ============================
// 初期化
// ============================
void Knight::DerivationInit()
{
	// モデルのロード
	m_Draw->LoadModel(m_ModelName.c_str(), "Character");

	// 位置を更新
	m_SRT.position = { 0.0f, 0.0f, 5.0f };
	m_SRT.scale = { 1.5f, 1.5f, 1.5f };

	// SRT行列
	Matrix4x4 world = m_SRT.UpdateWorldMatrix().toGPU();

	// 定数バッファ作成
	m_Draw->CreateConstantBuffer(
		m_TransformCBName.c_str(),
		&world,
		sizeof(world),
		BufferUsage::Dynamic,
		CPUAccess::Write);

	// オブジェクト情報設定
	m_ObjectInfo = Debug_AABB::COLLIDER_CONFIG.objectInfo;

	// AABBコライダーを毎フレーム更新
	m_AABBCol = CreateAABB(m_SRT, Debug_AABB::CENTER_OFFSET, Debug_AABB::HITBOX_SCALE);
}


// ============================
// 更新
// ============================
void Knight::Update()
{
	// 当たり判定の初期化
	m_IsHit = false;

	// 行列更新
	m_SRT.UpdateWorldMatrix();

	// AABBコライダーを毎フレーム更新
	m_AABBCol = CreateAABB(m_SRT, Debug_AABB::CENTER_OFFSET, Debug_AABB::HITBOX_SCALE);
}


// ============================
// 描画
// ============================
void Knight::Draw()
{
	// ワールド行列取得
	Matrix4x4 world = m_SRT.world.toGPU();

	// 定数バッファ更新
	m_Draw->UpdateShaderConstants(m_TransformCBName.c_str(), &world, sizeof(world));

	// 描画
	m_Draw->ModelDraw(m_VSName.c_str(), m_PSName.c_str(), m_ModelName.c_str());
}


// ============================
// 終了
// ============================
void Knight::Uninit()
{

}
