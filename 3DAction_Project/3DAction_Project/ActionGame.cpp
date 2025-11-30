
// ==========================================
// ヘッダー
// ==========================================
// 必須
#include "ActionGame.h"
// 計算
#include "Matrix4x4.h"
#include "Vector3.h"
// 時間
#include "Timer.h"
// 定数バッファ設定
#include "GraphicsEnums.h"
// メモリ
#include <memory>
// 文字列
#include <string>
// オブジェクト
#include "DCCCamera3D.h"
#include "Square2D.h"
#include "Knight.h"
// ログ出力
#include "ReportMessage.h"


// ==========================================
// 構造体　デバッグ用
// ==========================================
struct CameraInfo
{
	Matrix4x4 ViewMatrix;
	Matrix4x4 ProjMatrix;
};
// 四角形描画
Square2D g_Square;
Knight g_Knight;
// DCCカメラ
std::unique_ptr<BaseCamera> g_Camera;


// =================================
// 初期化
// =================================
bool ActionGame::DerivativeInit()
{
	// DCCカメラ初期化
	std::unique_ptr<DCCCamera3D> camera = std::make_unique<DCCCamera3D>();
	// カメラ初期化
	camera->SetInput(m_Input);
	g_Camera = std::move(camera);

	// カメラ初期化
	if (!g_Camera->Init(m_DrawManager)) {
		ErrorLog::OutputToConsole("DCCカメラの初期化に失敗");
		return false;
	}

	// プロジェクション行列（透視投影）
	float fov = 3.14159265f / 4.0f;
	float aspect = 1280.0f / 720.0f;
	float nearZ = 0.1f;
	float farZ = 100.0f;
	CameraProjInfo proj = { fov, aspect, nearZ, farZ };

	// カメラに設定
	g_Camera->SetProjection(proj);

	// 四角
	// g_Square.Init(m_DrawManager);
	g_Knight.Init(m_DrawManager);

	Timer::Init(); // タイマー初期化
	Timer::Start(); // タイマー開始

	return true;
}


// ================================
// 更新
// ================================
void ActionGame::Update()
{
	// タイマーデバッグ
	Timer::Debug_CheckUpdate();

	// g_Square.Update();
	g_Knight.Update();

	// カメラ更新
	g_Camera->Update();

	// 時間を取得
	m_FPSTime += Timer::GetDeltaTime();
	m_FPSCount++;

	if (m_FPSTime > 1.0f)
	{
		// ログ出力
		WarningLog::OutputToConsole(std::string("1フレームのFPS " + std::to_string(m_FPSCount)).c_str());

		// リセット
		m_FPSTime = 0.0f;
		m_FPSCount = 0;
	}

	// デバッグ用描画モード切り替え
	if (m_Input->GetMouseTrigger(Mouse_Left))
	{
		m_DrawManager->SetDrawSetting(FillModeSetting::Wireframe);
		m_CursorController->SetCursorMode(CursorMode::CursorMode_Hidden);
		DebugLog::OutputToConsole("ワイヤーフレームモード");
	}
	else if (m_Input->GetMouseRelease(Mouse_Left))
	{
		m_CursorController->SetCursorMode(CursorMode::CursorMode_Normal);
		m_DrawManager->SetDrawSetting(FillModeSetting::Solid);
		DebugLog::OutputToConsole("通常描画モード");
	}

	// タイマー更新処理
	Timer::LastUpdate();
}


// ================================
// 描画
// ================================
void ActionGame::Draw()
{
	// カメラ情報をGPUに送る
	g_Camera->UpdateToGPU();

	// 描画前
	m_DrawManager->BegingDraw();

	// g_Square.Draw();
	g_Knight.Draw();

	// 描画後
	m_DrawManager->EndDraw();
}


// ================================
// 後処理
// ================================
void ActionGame::Uninit()
{
	// g_Square.Uninit();
	g_Knight.Uninit();
	g_Camera->Uninit();
}
