
// ============================
// ヘッダー
// ============================
// 必須ヘッダー
#include "Player.h"
// 計算ヘッダー
#include "Vector3.h"
// 当たり判定
#include "AABBCollider.h"


// ============================
// オブジェクトの初期化
// ============================
void Player::LateInit()
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

#if defined(DEBUG) || defined(_DEBUG)
	// デバッグ用BOX初期化
	m_DebugBox.Init(m_Draw);
#endif
}


// ============================
// オブジェクトの更新
// ============================
void Player::Update()
{

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


#if defined(DEBUG) || defined(_DEBUG)
// ============================
// デバッグ用BOX描画
// ============================
void Player::DebugDrawBox()
{

}
#else
void Player::DebugDrawBox() {}
#endif
