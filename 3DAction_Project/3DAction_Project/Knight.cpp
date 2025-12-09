
// ============================
// ヘッダー
// ============================
// 必須ヘッダー
#include "Knight.h"
// 計算ヘッダー
#include "Vector3.h"


// ============================
// 初期化
// ============================
void Knight::DerivationInit()
{
	// モデルのロード
	m_Draw->LoadModel(m_ModelName.c_str(), "Character");

	// 位置を更新
	m_SRT.position = { 0.0f, 0.0f, 5.0f };
	m_SRT.scale = { 1.0f, 1.0f, 1.0f };

	// SRT行列
	Matrix4x4 world = m_SRT.UpdateWorldMatrix().toGPU();

	// 定数バッファ作成
	m_Draw->CreateConstantBuffer(
		m_TransformCBName.c_str(),
		&world,
		sizeof(world),
		BufferUsage::Dynamic,
		CPUAccess::Write);
}


// ============================
// 更新
// ============================
void Knight::DerivationUpdate()
{

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
