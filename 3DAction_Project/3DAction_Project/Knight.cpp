#include "Knight.h"

// ============================
// 初期化
// ============================
void Knight::LateInit()
{
	// モデルのロード
	m_Draw->LoadModel(m_ModelName.c_str());

	// 定数バッファ作成
	m_Draw->CreateConstantBuffer(
		m_TransformCBName.c_str(),
		&m_SRT,
		sizeof(m_SRT),
		BufferUsage::Dynamic,
		CPUAccess::Write);
}


// ============================
// 更新
// ============================
void Knight::Update()
{

}


// ============================
// 描画
// ============================
void Knight::Draw()
{
	// 描画
	Matrix4x4 world = m_SRT.toGPU();

	// 定数バッファ更新
	m_Draw->UpdateShaderConstants(m_TransformCBName.c_str(), &world, sizeof(world));

	//// 描画
	m_Draw->ModelDraw(m_VSName.c_str(), m_PSName.c_str(), m_ModelName.c_str());
}


// ============================
// 終了
// ============================
void Knight::Uninit()
{

}
