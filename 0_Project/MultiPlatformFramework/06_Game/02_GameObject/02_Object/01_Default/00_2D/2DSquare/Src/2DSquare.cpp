
// =====================================
// ヘッダー
// =====================================
#include "2DSquare.h"


// =====================================
// 初期化
// =====================================
void Square2D::DerivationInit()
{
	// 頂点バッファ作成
	m_Draw->CreateVertexBuffer(
		m_VsBufferName,
		m_Vertices,
		sizeof(Vertex),
		sizeof(m_Vertices) / sizeof(Vertex),
		sizeof(m_Vertices) / sizeof(Vertex),
		PrimitiveType::TriangleStrip,
		BufferUsage::Dynamic,
		CPUAccess::Write);

	// SRT情報からワールド行列計算
	m_SRT.UpdateWorldMatrix();
	Matrix4x4 world = m_SRT.world.toGPU();

	// 定数バッファ作成
	m_Draw->CreateConstantBuffer(
		m_TransformCBName,
		&world,
		sizeof(world),
		BufferUsage::Dynamic,
		CPUAccess::Write);

	// 外部画像をロード
	m_Draw->LoadTexture(m_TextureName);
}


// =====================================
// 更新
// =====================================
void Square2D::Update()
{
	// 更新
	m_SRT.UpdateWorldMatrix();
}


// =====================================
// 描画
// =====================================
void Square2D::Draw()
{
	// 定数バッファ更新
	Matrix4x4 world = m_SRT.world.toGPU();
	m_Draw->UpdateShaderConstants(m_TransformCBName, &world, sizeof(world));

	// 描画
	m_Draw->PrimitiveDraw(m_VSName, m_PSName, m_VsBufferName, m_TextureName);
}


// =====================================
// 後処理
// =====================================
void Square2D::Uninit()
{

}
