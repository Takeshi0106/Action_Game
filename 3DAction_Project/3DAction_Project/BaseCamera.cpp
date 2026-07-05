
// =================================
// ヘッダー
// =================================
// 必須ヘッダー
#include "BaseCamera.h"
// ログ出力ヘッダー
#include "ReportMessage.h"


// =================================
// 派生初期化
// =================================
bool BaseCamera::Init(BaseDrawManager* drawManager)
{
	// 描画マネージャー保存
	m_DrawManager = drawManager;

	// 初期プロジェクション設定
	SetProjection(m_ProjInfo);

	// GPUに送る用に変換
	Matrix4x4 toGPU[2] = {
		Matrix4x4::CreateViewMatrix_LH(
			m_ViewInfo.CameraPos,
			m_ViewInfo.LookPoint,
			m_ViewInfo.UpDir),
		m_ProjMatrix
	};

	// カメラの定数バッファ作成
	if (!m_DrawManager->CreateConstantBuffer(
		kCameraInfoBufferName,
		toGPU,
		sizeof(toGPU),
		BufferUsage::Dynamic,
		CPUAccess::Write))
	{
		ErrorLog::OutputToConsole(u8"カメラ定数バッファの作成に失敗");
		return false;
	}

	// 派生クラス初期化
	if (!DerivativInit())
	{
		ErrorLog::OutputToConsole(u8"派生クラスの初期化に失敗");
		return false;
	}

	return true;
}


// =================================
// カメラ情報をGPUに送る関数
// =================================
void BaseCamera::UpdateToGPU()
{
	// ビュー行列計算
	Matrix4x4 viewMatrix = Matrix4x4::CreateViewMatrix_LH(
		m_ViewInfo.CameraPos,
		m_ViewInfo.LookPoint,
		m_ViewInfo.UpDir);

	// GPUに送る用に変換
	Matrix4x4 toGPU[2] = {
		viewMatrix,
		m_ProjMatrix
	};

	// GPUに送る
	m_DrawManager->UpdateShaderConstants(
		kCameraInfoBufferName,
		&toGPU,
		sizeof(toGPU));

	// 派生クラスのGPU更新処理
	DerivativUpdateToGPU();
}
