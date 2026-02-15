
// ============================================================================
// ヘッダー
// ============================================================================
// 必須ヘッダー
#include "DirectX11_SamplerManager.h"
// ログ出力用ヘッダー
#include "../../../ReportMessage.h"


// ============================================================================
// サンプラー作成
// ============================================================================
Handle DirectX11_SamplerManager::SamplerStateCreateOnGet(
	ID3D11Device* _device,
	const D3D11_SAMPLER_DESC& _dxDesc,
	const SamplerDesc& _myDesc)
{
	if (m_Samplers.Exists(_myDesc))
	{
		// 取得
		Handle handle = m_Samplers.GetHandle(_myDesc);

#if defined(DEBUG) || defined(_DEBUG)
		// 同一確認
		if (!m_Samplers.GetData(handle)->IsSame(_dxDesc)) {
			WarningLog::OutputToConsole(u8"同じ設定のサンプラーが異なる設定で作成されようとしました");
			return Handle();
		}
#endif
		// ログ出力
		WarningLog::OutputToConsole(u8"同じ設定のサンプラーが作成されようとしました");
		return handle;
	}

	// 無効チェック
	if (!_device) {
		ErrorLog::OutputToConsole(u8"無効なサンプラーが作成されそうになりました");
		return Handle();
	}

	// サンプラー作成
	DirectX11_SamplerData sampler;

	// サンプラー作成
	if (!sampler.Create_DX11SamplerState(_device, _dxDesc)) {
		ErrorLog::OutputToConsole(u8"サンプラーの作成に失敗しました");
		return Handle();
	}

	// 管理配列に追加してハンドルを返す
	return m_Samplers.AddData(_myDesc, sampler);
}
