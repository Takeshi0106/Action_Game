
// =====================================
// ヘッダー
// =====================================
#include "2DTriangle.h"
#include "Quaternionh.h"
#include "Timer.h"

// グローバル変数
float g_angle = 0.0f;


// =====================================
// 初期化
// =====================================
void Triangle2D::DerivationInit()
{
	// 頂点バッファ作成
	m_Draw->CreateVertexBuffer(
		m_VsBufferName.c_str(),
		m_Vertices,
		sizeof(Vertex),
		sizeof(m_Vertices) / sizeof(Vertex),
		sizeof(m_Vertices) / sizeof(Vertex),
		PrimitiveType::TriangleStrip,
		BufferUsage::Dynamic,
		CPUAccess::Write);

	// 行列SRT作成
	Matrix4x4 world = Matrix4x4::CreateIdentityMatrix().toGPU();

	// ワールド作成
	m_Draw->CreateConstantBuffer(
		m_TransformCBName.c_str(),
		&world,
		sizeof(world),
		BufferUsage::Dynamic,
		CPUAccess::Write);
}


// =====================================
// 更新
// =====================================
void Triangle2D::Update()
{
	// デルタタイム取得
	float time = Timer::GetDeltaTime();

	// Y軸周りに回転させるとします 
	float speed = 3.14159265f * 4;

	// 経過時間に応じて角度を増加 
	g_angle += speed * time;

	// ワールド行列（回転のみ）
	Quaternion rotQuat = Quaternion::CreateQuaternionFromAxisAngle(Vector3(1, 0, 0), g_angle);
	m_SRT.rotation = m_SRT.rotation * rotQuat;

	// 移動
	static float offset = 0.0f;
	offset += 1.0f * time; // 時間経過で移動

	Matrix4x4 translationMatrix = Matrix4x4::CreateTranslationMatrix_LH(Vector3(offset, 0.0f, 0.0f));
	m_SRT.position.x = offset;

	// ワールド行列更新
	m_SRT.UpdateWorldMatrix();
}


// =====================================
// 描画
// =====================================
void Triangle2D::Draw()
{
	// GPUように変換
	Matrix4x4 world = m_SRT.world.toGPU();

	// 定数バッファ更新
	m_Draw->UpdateShaderConstants(m_TransformCBName.c_str(), &world, sizeof(world));

	// 描画
	m_Draw->PrimitiveDraw(m_VsName.c_str(), m_PsName.c_str(), m_VsBufferName.c_str());
}


// =====================================
// 後処理
// =====================================
void Triangle2D::Uninit()
{

}
