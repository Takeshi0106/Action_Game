#include "Knight.h"
#include "Vector3.h"


// ============================
// 初期化
// ============================
void Knight::LateInit()
{
	// モデルのロード
	m_Draw->LoadModel(m_ModelName.c_str(), "Character");

	// 定数バッファ作成
	m_Draw->CreateConstantBuffer(
		m_TransformCBName.c_str(),
		&m_SRT,
		sizeof(m_SRT),
		BufferUsage::Dynamic,
		CPUAccess::Write);

	// 位置を更新
	m_SRT = Matrix4x4::CreateTranslationMatrix_LH(Vector3(0.0f, 0.0f, 10.0f)) * m_SRT;
}


// ============================
// 更新
// ============================
void Knight::Update()
{
	m_SRT = m_SRT * Matrix4x4::CreateRotationYMatrix_LH(0.01f);
}


// ============================
// 描画
// ============================
void Knight::Draw()
{
	Matrix4x4 world = m_SRT.toGPU();

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
