
// ===============================
// ヘッダー
// ===============================
// 必須ヘッダー
#include "DebugScene.h"
// カメラヘッダー
#include "DCCCamera3D.h"
// ライトヘッダー
#include "SunLight.h"
// ログ出力ヘッダー
#include "ReportMessage.h"


// ==============================
// シーンの初期化
// ==============================
bool DebugScene::DerivativeInit()
{
	// DCCカメラ初期化
	std::unique_ptr<DCCCamera3D> camera = std::make_unique<DCCCamera3D>();
	// カメラ初期化
	camera->SetInput(m_Modules->input);
	m_Camera = std::move(camera);

	// カメラ初期化
	if (!m_Camera->Init(m_Modules->drawManager)) {
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
	m_Camera->SetProjection(proj);

	// 太陽光初期化
	m_Light = std::make_unique<SunLight>();
	// ライト初期化
	if (!m_Light->Init(m_Modules->drawManager)) {
		ErrorLog::OutputToConsole("太陽光の初期化に失敗");
		return false;
	}

	// オブジェクト初期化
	m_Square.Init(m_Modules->drawManager);
	m_Knight.Init(m_Modules->drawManager);

	return true;
}


// =============================
// シーンの更新
// =============================
void DebugScene::Update(float _deltaTime)
{
	// デバッグ用描画モード切り替え
	if (m_Modules->input->GetMouseTrigger(Mouse_Left))
	{
		m_Modules->drawManager->SetDrawSetting(FillModeSetting::Wireframe);
		m_Modules->cursor->SetCursorMode(CursorMode::CursorMode_Hidden);
		DebugLog::OutputToConsole("ワイヤーフレームモード");
	}
	else if (m_Modules->input->GetMouseRelease(Mouse_Left))
	{
		m_Modules->drawManager->SetDrawSetting(FillModeSetting::Solid);
		m_Modules->cursor->SetCursorMode(CursorMode::CursorMode_Normal);
		DebugLog::OutputToConsole("通常描画モード");
	}

	// カメラ更新
	m_Camera->Update();
	// ライト更新
	m_Light->Update();

	// オブジェクト更新
	m_Square.Update();
	m_Knight.Update();
}


// =============================
// シーンの描画
// =============================
void DebugScene::Draw()
{
	// カメラ情報をGPUに送る
	m_Camera->UpdateToGPU();
	// ライト情報をGPUに送る
	m_Light->UpdateToGPU();

	// オブジェクト描画
	m_Modules->drawManager->SetDepthStencilSetting(DepthStencilSetting::DepthEnableON_DepthWriteON);
	m_Knight.Draw();

	// 透明物書き込み
	m_Modules->drawManager->SetDepthStencilSetting(DepthStencilSetting::DepthEnableON_DepthWriteOFF);
	m_Square.Draw();
}


// =============================
// シーンの終了処理
// =============================
void DebugScene::Uninit()
{
	// カメラ後処理
	m_Camera->Uninit();
	// ライト後処理
	m_Light->Uninit();

	// オブジェクト後処理
	m_Square.Uninit();
	m_Knight.Uninit();
}
