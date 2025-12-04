
// ===============================
// ヘッダー
// ===============================
// 必須ヘッダー
#include "SunLight.h"
// ログ出力ヘッダー
#include "ReportMessage.h"


// ===============================
// 構造体
// ===============================
struct SunLightCB {
	// 光の情報
	Color Diffuse;
	Color Ambient;
	Color Specular;
	// 光の方向
	Vector3 Direction;
};


// ===============================
// 初期化
// ===============================
bool SunLight::DerivativeInit()
{
	// 光の方向設定
	m_Diffuse = Color(1.0f, 0.95f, 0.85f, 1.0f);
	m_Ambient = Color(0.25f, 0.3f, 0.35f, 1.0f);
	m_Specular = Color(1.0f, 1.0f, 1.0f, 1.0f);
	m_Direction = Vector3(1.0f, -1.0f, 1.0f).Normalize();

	// 定数バッファ用データ作成
	SunLightCB cbData = {
		m_Diffuse,
		m_Ambient,
		m_Specular,
		m_Direction};

	// 定数バッファ作成
	if (!m_Draw->CreateConstantBuffer(
		m_CBName,
		&cbData,
		sizeof(SunLightCB),
		BufferUsage::Dynamic,
		CPUAccess::Write))
	{
		ErrorLog::OutputToConsole("太陽光の定数バッファ作成に失敗");
		return false;
	}

	return true;
}


// ===============================
// 更新
// ===============================
void SunLight::Update()
{

}


// ===============================
// GPU転送
// ===============================
void SunLight::UpdateToGPU()
{
	// 定数バッファ用データ作成
	SunLightCB cbData = {
		m_Diffuse,
		m_Ambient,
		m_Specular,
		m_Direction	};

	// 定数バッファ更新
	m_Draw->UpdateShaderConstants(m_CBName, &cbData, sizeof(SunLightCB));
}


// ===============================
// 終了処理
// ===============================
void SunLight::Uninit()
{

}
