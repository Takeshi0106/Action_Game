
// ============================================================================
// ヘッダー
// ============================================================================
// 必須ヘッダー
#include "DirectX11_SamplerManager.h"
// ログ出力用ヘッダー
#include "ReportMessage.h"


// ============================================================================
// サンプラー作成
// ============================================================================
const Handle DirectX11_SamplerManager::SamplerStateCreate(
	ID3D11Device* _device,
	D3D11_SAMPLER_DESC& desc,
	const char* _name)
{

	if (!_device) {
		ErrorLog::OutputToConsole("無効なサンプラーが作成されそうになりました");
		return Handle();
	}

	// サンプラー作成
	Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler;

	// サンプラー作成
	HRESULT hr = _device->CreateSamplerState(&desc, sampler.GetAddressOf());
	if (FAILED(hr)) {
		return Handle();
	}

	// 管理配列に追加してハンドルを返す
	return m_Samplers.AddData(_name, sampler);
}


// ============================================================================
// サンプラー取得
// ============================================================================
ID3D11SamplerState* DirectX11_SamplerManager::GetSamplerState(const Handle& _handle)
{
	return m_Samplers.GetData(_handle)->Get();
}
