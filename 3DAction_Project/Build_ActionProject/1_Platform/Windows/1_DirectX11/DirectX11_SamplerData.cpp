
// ===========================================
// ヘッダー
// ===========================================
// 必須ヘッダー
#include "DirectX11_SamplerData.h"
// レポートメッセージヘッダー
#include "ReportMessage.h"


// ===========================================
// サンプラー作成
// ===========================================
bool DirectX11_SamplerData::Create_DX11SamplerState(
	ID3D11Device* _device,
	const D3D11_SAMPLER_DESC& _desc)
{
	// サンプラーステート作成
	HRESULT hr = _device->CreateSamplerState(
		&_desc,
		m_Sampler.GetAddressOf());

	// エラーチェック
	if (FAILED(hr)) {
		ErrorLog::OutputToConsole(u8"サンプラーステートの作成に失敗しました " +
			String::to_u8string(hr));
		return false;
	}

#if defined(DEBUG) || defined(_DEBUG)
	// デバッグ用情報取得
	m_Desc = _desc;
#endif

	return true;
}


#if defined(DEBUG) || defined(_DEBUG)
// ===========================================
// デバッグ時の同一確認
// ===========================================
bool DirectX11_SamplerData::IsSame(
	const D3D11_SAMPLER_DESC& _desc) const
{
	// フィルターが異なる
	if (m_Desc.Filter != _desc.Filter) {
		return false;
	}
	// アドレスモードが異なる
	if (m_Desc.AddressU != _desc.AddressU ||
		m_Desc.AddressV != _desc.AddressV ||
		m_Desc.AddressW != _desc.AddressW) {
		return false;
	}
	// ミップ LOD バイアスが異なる
	if (m_Desc.MipLODBias != _desc.MipLODBias) {
		return false;
	}
	// 最大異方性が異なる
	if (m_Desc.MaxAnisotropy != _desc.MaxAnisotropy) {
		return false;
	}
	// 比較関数が異なる
	if (m_Desc.ComparisonFunc != _desc.ComparisonFunc) {
		return false;
	}
	// ボーダーカラーが異なる
	for (int i = 0; i < 4; ++i) {
		if (m_Desc.BorderColor[i] != _desc.BorderColor[i]) {
			return false;
		}
	}
	// 最小 LOD が異なる
	if (m_Desc.MinLOD != _desc.MinLOD) {
		return false;
	}
	// 最大 LOD が異なる
	if (m_Desc.MaxLOD != _desc.MaxLOD) {
		return false;
	}
	return true;
}

#endif
