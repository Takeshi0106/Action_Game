
// ============================
// ヘッダー
// ============================
// 必須ヘッダー
#include "Player.h"
// 計算ヘッダー
#include "Vector3.h"
// 当たり判定
#include "ColliderConfig.h"
#include "AABBCollider.h"


// ============================
// 定数
// ============================
namespace Player_AABB{
	// コライダーオフセット
	constexpr Vector3 CENTER_OFFSET = { 0.0f,1.5f,0.0f };
	// ヒットボックススケール
	constexpr Vector3 HITBOX_SCALE = { 3.5f, 3.0f, 2.8f };

	// プレイヤーのコライダー設定
	constexpr ColliderPresetConfig COLLIDER_CONFIG = {
		CENTER_OFFSET,
		HITBOX_SCALE,
		ColliderShapeType::AABB,
		ObjectInfo{ 0, ObjectTag::PLAYER } };
}


// ============================
// オブジェクトの初期化
// ============================
void Player::DerivationInit()
{
	// モデルのロード
	m_Draw->LoadModel(m_ModelName.c_str(), "Character");

	// 位置を更新
	m_SRT.position = { 0.0f, 0.0f, 5.0f };
	m_SRT.scale = { 1.0f, 1.0f, 1.0f };

	// SRT行列
	m_SRT.UpdateWorldMatrix();
	Matrix4x4 world = m_SRT.world.toGPU();

	// 定数バッファ作成
	m_Draw->CreateConstantBuffer(
		m_TransformCBName.c_str(),
		&world,
		sizeof(world),
		BufferUsage::Dynamic,
		CPUAccess::Write);

	// オブジェクト情報設定
	m_ObjectInfo = Player_AABB::COLLIDER_CONFIG.objectInfo;
}


// ============================
// オブジェクトの更新
// ============================
void Player::Update()
{
	// 当たり判定の初期化
	m_IsHit = false;


	// AABBコライダーを毎フレーム更新
	m_AABBCol = CreateAABB(m_SRT, Player_AABB::CENTER_OFFSET, Player_AABB::HITBOX_SCALE);
}


// ============================
// オブジェクトの描画
// ============================
void Player::Draw()
{
	// SRVT行列更新
	m_SRT.UpdateWorldMatrix();
	Matrix4x4 world = m_SRT.world.toGPU();

	// 定数バッファ更新
	m_Draw->UpdateShaderConstants(m_TransformCBName.c_str(), &world, sizeof(world));

	// 描画
	m_Draw->ModelDraw(m_VSName.c_str(), m_PSName.c_str(), m_ModelName.c_str());
}


// ============================
// オブジェクトの後処理
// ============================
void Player::Uninit()
{

}
