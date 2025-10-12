#include "ActionGame.h"
#include  "Quaternionh.h"
#include "Matrix4x4.h"
#include "Vector3.h"
#include "Color.h"
#include "Timer.h"
#include "GraphicsEnums.h"
#include <string>

#include "2DTriangle.h"

// ==========================================
// 構造体　デバッグ用
// ==========================================
struct CameraInfo
{
	Matrix4x4 ViewMatrix;
	Matrix4x4 ProjMatrix;
};

Triangle2D g_Triangle;


// =================================
// 初期化
// =================================
void ActionGame::Init(BaseDrawManager* _drawManager)
{
	m_DrawManager = _drawManager;

	// 定数バッファ初期化
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

	CameraInfo mat = { view.toGPU(),proj.toGPU() };

	// 定数バッファ作成
	m_DrawManager->CreateConstantBuffer(
		"CameraInfo",
		&mat,
		sizeof(mat),
		BufferUsage::Dynamic,
		CPUAccess::Write);

	// Triangle初期化
	g_Triangle.Init(m_DrawManager);

	// 外部画像をロード
	m_DrawManager->LoadTexture("Asset/Texture/23249532.jpg");


	Timer::Init(); // タイマー初期化
	Timer::Start(); // タイマー開始
}


// ================================
// 更新
// ================================
void ActionGame::Update()
{
	// タイマーデバッグ
	Timer::Debug_CheckUpdate();

	// Triangle更新
	g_Triangle.Update();

	// タイマー更新処理
	Timer::LastUpdate();
}


// ================================
// 描画
// ================================
void ActionGame::Draw()
{
	// 描画前
	m_DrawManager->BegingDraw();

	// Triangle 描画
	g_Triangle.Draw();

	// 描画後
	m_DrawManager->EndDraw();
}


// ================================
// 後処理
// ================================
void ActionGame::Uninit()
{
	// Triangle 後処理
	g_Triangle.Uninit();
}