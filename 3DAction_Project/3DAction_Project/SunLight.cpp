
// ===============================
// ヘッダー
// ===============================
// 必須ヘッダー
#include "SunLight.h"
// ログ出力ヘッダー
#include "ReportMessage.h"

#if defined(DEBUG) || defined(_DEBUG)
// Imgui用ヘッダー
#include "4_Imgui/imgui.h"
#endif


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
		ErrorLog::OutputToConsole(u8"太陽光の定数バッファ作成に失敗");
		return false;
	}

	return true;
}


// ===============================
// 更新
// ===============================
void SunLight::Update()
{

	// デバッグ用
	DebugImgui();
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



#if defined(DEBUG) || defined(_DEBUG)
// ===============================
// デバッグ用imgui関数
// ===============================
void SunLight::DebugImgui()
{
	// ウィンドウ開始
	ImGui::Begin("SunLight");

	// 光の色スライダー
	float diffuse[4] = { m_Diffuse.r, m_Diffuse.g, m_Diffuse.b, m_Diffuse.a };
	if (ImGui::ColorEdit4("Diffuse", diffuse))
	{
		m_Diffuse = Color(diffuse[0], diffuse[1], diffuse[2], diffuse[3]);
	}
	// 環境光スライダー
	float ambient[4] = { m_Ambient.r, m_Ambient.g, m_Ambient.b, m_Ambient.a };
	if (ImGui::ColorEdit4("Ambient", ambient))
	{
		m_Ambient = Color(ambient[0], ambient[1], ambient[2], ambient[3]);
	}
	// 鏡面反射光スライダー
	float specular[4] = { m_Specular.r, m_Specular.g, m_Specular.b, m_Specular.a };
	if (ImGui::ColorEdit4("Specular", specular))
	{
		m_Specular = Color(specular[0], specular[1], specular[2], specular[3]);
	}
	// 光の方向スライダー
	float dir[3] = { m_Direction.x, m_Direction.y, m_Direction.z };
	if (ImGui::SliderFloat3("Direction", dir, -1.0f, 1.0f))
	{
		m_Direction = Vector3(dir[0], dir[1], dir[2]).Normalize();
	}

	// ウィンドウ終了
	ImGui::End();
}
#else
void SunLight::DebugImgui() {}
#endif
