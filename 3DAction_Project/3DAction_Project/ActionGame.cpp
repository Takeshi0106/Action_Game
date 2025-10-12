#include "ActionGame.h"
#include  "Quaternionh.h"
#include "Matrix4x4.h"
#include "Vector3.h"
#include "Color.h"
#include "Timer.h"
#include "GraphicsEnums.h"
#include <string>

// ==========================================
// 構造体　デバッグ用
// ==========================================
struct Vertex {
	Vector3 pos;
	Color color;
};
struct CameraInfo
{
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

std::string VsName = "VS_TriangleDebug";
std::string PsName = "PS_TriangleDebug";


// =================================
// 初期化
// =================================
void ActionGame::Init(BaseDrawManager* _drawManager)
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

	CameraInfo mat = { view.toGPU(),proj.toGPU() };

	// 頂点バッファ作成
	m_DrawManager->CreateVertexBuffer(
		"VS_TriangleDebug",
		vertices,
		sizeof(Vertex),
		sizeof(vertices)/sizeof(Vertex),
		PrimitiveType::TriangleStrip,
		BufferUsage::Dynamic,
		CPUAccess::Write);

	// 定数バッファ作成
	m_DrawManager->CreateConstantBuffer(
		"CameraInfo",
		&mat,
		sizeof(mat),
		BufferUsage::Dynamic,
		CPUAccess::Write);

	// ワールド作成
	m_DrawManager->CreateConstantBuffer(
		"Transform1",
		&world,
		sizeof(world),
		BufferUsage::Dynamic,
		CPUAccess::Write);

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

	// デルタタイム取得
	float deltaTime = Timer::GetDeltaTime();

	// Y軸周りに回転させるとします 
	float speed = 3.14159265f * 4;

	// 経過時間に応じて角度を増加 
	angle += speed * deltaTime;

	// ワールド行列（回転のみ）
	Quaternion rotQuat = Quaternion::CreateQuaternionFromAxisAngle(Vector3(1, 0, 0), angle);
	Matrix4x4 rotationMatrix = Matrix4x4::CreateRotationQuaternion_LH(rotQuat);

	// 移動
	static float offset = 0.0f;
	offset += 1.0f * deltaTime; // 時間経過で移動

	Matrix4x4 translationMatrix = Matrix4x4::CreateTranslationMatrix_LH(Vector3(offset, 0.0f, 0.0f));

	// ワールド行列
	Matrix4x4 world = translationMatrix * rotationMatrix;

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

	world = world.toGPU();

	// 定数バッファ更新
	m_DrawManager->UpdateShaderConstants("CameraInfo", &mat, sizeof(mat));

	m_DrawManager->UpdateShaderConstants("Transform1", &world, sizeof(world));

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
	
	// 描画
	m_DrawManager->Draw(VsName.c_str(), PsName.c_str(), "", "");

	// 描画後
	m_DrawManager->EndDraw();
}


// ================================
// 後処理
// ================================
void ActionGame::Uninit()
{

}