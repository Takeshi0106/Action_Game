#include "GameMain.h"
#include "Matrix4x4.h"
#include "Vector3.h"
#include "Color.h"
#include "Timer.h"


// ==========================================
// 構造体　デバッグ用
// ==========================================
/*
struct Vertex {
	Vector3 pos;
	Color color;
};
struct TransformCB
{
	Matrix4x4 WorldMatrix;
	Matrix4x4 ViewMatrix;
	Matrix4x4 ProjMatrix;
};
// 三角形の頂点
Vertex vertices[3] =
{
{ { 0.0f, 0.57735f, 0.0f }, {1, 0, 0, 1} },   // 上頂点 (y = √3/2 * 0.5)
{ { 0.5f, -0.288675f, 0.0f }, {0, 1, 0, 1} }, // 右下頂点
{ { -0.5f, -0.288675f, 0.0f }, {0, 0, 1, 1} } // 左下頂点
};
float angle;


void GameMain::Init(BaseDrawManager* _drawManager)
{
	m_DrawManager = _drawManager;

	// 定数バッファ初期化
	// ワールド行列
	Matrix4x4 world = Matrix4x4::CreateIdentityMatrix();

	// ビュー行列（カメラを少し離す）
	Vector3 eye(0, 0, -5);   // カメラ位置
	Vector3 at(0, 0, 0);     // 注視点
	Vector3 up(0, 1, 0);     // 上方向
	Matrix4x4 view = Matrix4x4::CreateViewMatrix_LH(eye, at, up);

	// プロジェクション行列（透視投影）
	float fov = 3.14159265f / 4.0f;     // 視野角45°
	float aspect = 1280.0f / 720.0f;
	float nearZ = 0.1f;
	float farZ = 100.0f;
	Matrix4x4 proj = Matrix4x4::CreateProjectionMatrix_LH(fov, aspect, nearZ, farZ);

	TransformCB mat = { world.toGPU(), view.toGPU(),proj.toGPU() };


	// 頂点バッファ作成
	m_DrawManager->CreateVertexBuffer(
		"VS_TriangleDebug",
		vertices,
		sizeof(Vertex),
		PrimitiveType::TriangleStrip,
		BufferUsage::Dynamic,
		CPUAccess::Write);

	// 定数バッファ作成
	m_DrawManager->CreateConstantBuffer(
		"Transform1",
		&mat,
		sizeof(mat),
		BufferUsage::Dynamic,
		CPUAccess::Write);


	Timer::Init(); // タイマー初期化
	Timer::Start(); // タイマー開始
}
*/