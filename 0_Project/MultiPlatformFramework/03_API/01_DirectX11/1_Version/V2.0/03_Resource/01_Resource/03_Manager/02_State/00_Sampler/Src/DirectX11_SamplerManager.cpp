
// ヘッダー
#include "DirectX11_SamplerManager.h"
#include "ReportMessage.h"

// サンプラー作成
Handle DirectX11_SamplerManager::SamplerStateCreateOnGet(
	ID3D11Device* _device,
	const D3D11_SAMPLER_DESC& _dxDesc,
	const SamplerDesc& _myDesc)
{
	// 同じ設定のサンプラーが存在するかチェック
	if (m_Samplers.Exists(_myDesc))
	{
		Handle handle = m_Samplers.GetHandle(_myDesc);
#if defined(DEBUG) || defined(_DEBUG)
		// 同一確認
		if (!m_Samplers.GetData(handle)->IsSame(_dxDesc)) {
			WarningLog::OutputToConsole(u8"同じ設定のサンプラーが異なる設定で作成されようとしました");
			return Handle();
		}
#endif
		WarningLog::OutputToConsole(u8"同じ設定のサンプラーが作成されようとしました");
		return handle;
	}

	// サンプラー
	DirectX11_SamplerData sampler;
	if (!sampler.Create_DX11SamplerState(_device, _dxDesc)) {
		ErrorLog::OutputToConsole(u8"サンプラーの作成に失敗しました");
		return Handle();
	}

	// 管理配列に追加してハンドルを返す
	return m_Samplers.AddData(_myDesc, sampler);
}
