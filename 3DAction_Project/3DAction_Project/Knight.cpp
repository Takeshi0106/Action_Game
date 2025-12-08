
// ============================
// ヘッダー
// ============================
// 必須ヘッダー
#include "Knight.h"
// 計算ヘッダー
#include "Vector3.h"
// 当たり判定
#include "AABBCollider.h"


// ============================
// 初期化
// ============================
void Knight::LateInit()
{
	// モデルのロード
	m_Draw->LoadModel(m_ModelName.c_str(), "Character");

	// SRT行列
	CreateWorldMatrix();
	Matrix4x4 world = m_WorldMatrix.toGPU();

	// 定数バッファ作成
	m_Draw->CreateConstantBuffer(
		m_TransformCBName.c_str(),
		&world,
		sizeof(world),
		BufferUsage::Dynamic,
		CPUAccess::Write);

	// 位置を更新
	m_Position = { 0.0f, 0.0f, 5.0f };
	m_Scale = { 1.0f, 1.0f, 1.0f };

	// 当たり判定作成
	AABBCollider collider = CreateAABB(m_Scale.x, m_Scale.y, m_Scale.z);
	collider.offset = { 0.0f, m_Scale.y + 0.5f, -m_Scale.z * 0.5f };
	collider.offsetSize = { 3.5f,3.0f,2.8f };

	// コライダー情報取得
	m_AABBColliderInfo = m_Collider->ColliderAdd("Knight_AABB", collider);

#if defined(DEBUG) || defined(_DEBUG)
	// デバッグ用BOX初期化
	m_DebugBox.Init(m_Draw);
#endif
}


// ============================
// 更新
// ============================
void Knight::Update()
{
	// m_SRT = m_SRT * Matrix4x4::CreateRotationYMatrix_LH(0.01f);
}


// ============================
// 描画
// ============================
void Knight::Draw()
{
	CreateWorldMatrix();
	Matrix4x4 world = m_WorldMatrix.toGPU();

	// 定数バッファ更新
	m_Draw->UpdateShaderConstants(m_TransformCBName.c_str(), &world, sizeof(world));

	// 描画
	m_Draw->ModelDraw(m_VSName.c_str(), m_PSName.c_str(), m_ModelName.c_str());

	// デバッグ用BOX描画
	// DebugDrawBox();
}


// ============================
// 終了
// ============================
void Knight::Uninit()
{

}



#if defined(DEBUG) || defined(_DEBUG)
// ============================
// デバッグ用BOX描画
// ============================
void Knight::DebugDrawBox()
{
	// 当たり判定更新
	AABBCollider* aabb = m_Collider->GetAABBCollider(m_AABBColliderInfo.ID);

	// 位置・サイズ更新
	AABBCollider newaaabb = aabb->UpdateAABB(m_Position, m_Scale);

	// BOX情報更新
	m_DebugBox.DrawAABB(m_Draw, &newaaabb, Color(1.0f, 1.0f, 1.0f, 0.5f));
}
#else
void Knight::DebugDrawBox() {}
#endif
