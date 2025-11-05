
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
void Triangle2D::Init(BaseDrawManager* _drawManager)
{
	// オブジェクトの初期化
	BaseObject::Init(_drawManager);

	// 頂点バッファ作成
	m_Draw->CreateVertexBuffer(
		"VS_TriangleDebug",
		m_Vertices,
		sizeof(Vertex),
		sizeof(m_Vertices) / sizeof(Vertex),
		PrimitiveType::TriangleStrip,
		BufferUsage::Dynamic,
		CPUAccess::Write);

	// ワールド作成
	m_Draw->CreateConstantBuffer(
		"Transform1",
		&m_SRT,
		sizeof(m_SRT),
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
	Matrix4x4 rotationMatrix = Matrix4x4::CreateRotationQuaternion_LH(rotQuat);

	// 移動
	static float offset = 0.0f;
	offset += 1.0f * time; // 時間経過で移動

	Matrix4x4 translationMatrix = Matrix4x4::CreateTranslationMatrix_LH(Vector3(offset, 0.0f, 0.0f));

	// ワールド行列
	Matrix4x4 world = translationMatrix * rotationMatrix;

	// 代入
	m_SRT = world;
}


// =====================================
// 描画
// =====================================
void Triangle2D::Draw()
{
	// GPUように変換
	Matrix4x4 world = m_SRT.toGPU();

	// 定数バッファ更新
	m_Draw->UpdateShaderConstants("Transform1", &world, sizeof(world));

	// 描画
	m_Draw->Draw(VsName.c_str(), PsName.c_str());	
}


// =====================================
// 後処理
// =====================================
void Triangle2D::Uninit()
{

}