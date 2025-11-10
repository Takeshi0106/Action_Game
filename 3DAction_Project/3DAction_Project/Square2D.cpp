
// =====================================
// ヘッダー
// =====================================
#include "Square2D.h"
#include "Quaternionh.h"
#include "Timer.h"


// =====================================
// 初期化
// =====================================
void Square2D::Init(BaseDrawManager* _drawManager)
{
	// オブジェクトの初期化
	BaseObject::Init(_drawManager);

	// 頂点バッファ作成
	m_Draw->CreateVertexBuffer(
		m_VSName.c_str(),
		m_Vertices,
		sizeof(Vertex),
		sizeof(m_Vertices) / sizeof(Vertex),
		PrimitiveType::TriangleStrip,
		BufferUsage::Dynamic,
		CPUAccess::Write);

	// 定数バッファ作成
	m_Draw->CreateConstantBuffer(
		m_CBName.c_str(),
		&m_SRT,
		sizeof(m_SRT),
		BufferUsage::Dynamic,
		CPUAccess::Write);

	// 外部画像をロード
	m_Draw->LoadTexture(m_TextureName.c_str());
}


// =====================================
// 更新
// =====================================
void Square2D::Update()
{

}


// =====================================
// 描画
// =====================================
void Square2D::Draw()
{
	// GPUように変換
	Matrix4x4 world = m_SRT.toGPU();

	// 定数バッファ更新
	m_Draw->UpdateShaderConstants(m_CBName.c_str(), &world, sizeof(world));

	// 描画
	m_Draw->Draw(m_VSName.c_str(), m_PSName.c_str(), m_TextureName.c_str());
}


// =====================================
// 後処理
// =====================================
void Square2D::Uninit()
{

}