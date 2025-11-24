
// ============================
// ヘッダー
// ============================
// 必須ヘッダー
#include "ResourceViewManager.h"
// Dataヘッダー
#include "ResourceViewData.h"
// 変換ヘッダー
#include "DirectX_FormatConverter.h"
// ログ出力
#include "ReportMessage.h"


// =============================
// SRV
// =============================
// 作成
bool ResourceViewManager::CreateSRV(const std::string& name,
	ID3D11Device* device,
	ID3D11Resource* resource,
	Format format,
	UINT mostDetailedMip,
	UINT mipLevels)
{
	// 情報を構造体にまとめてマップに保存
	auto srv = std::make_unique<SRVData>();

	// バッファ作成
	if (!srv->CreateSRV(
		device,
		resource,
		DirectX_FormatConverter::ToDXFormat(format),
		mostDetailedMip,
		mipLevels))
	{
		ErrorLog::OutputToConsole(std::string(("SRV の作成失敗: " + name)).c_str());
		return false;
	}

	// 配列に代入
	m_SRVs[name] = std::move(srv);

	// デバッグ用に名前を保存しておく
	m_Logger.Log(("SRV : " + name).c_str());

	return true;
}

// ゲッター
bool ResourceViewManager::BindSRV(const std::string& name, ID3D11DeviceContext* context, SETSHADERTYPE type)
{
	// 探す
	auto it = m_SRVs.find(name);

	if (it != m_SRVs.end()) {
		
		// 保存用 のちに配列対応
		ID3D11ShaderResourceView* srv = it->second.get()->GetSRV();

		// バインド
		switch (type)
		{
		case VERTEXSHADER:
			context->VSSetShaderResources(0, 1, &srv);
			break;
		case PIXSELSHADER:
			context->PSSetShaderResources(0, 1, &srv);
			break;
		case CONPUTESHADER:
			context->CSSetShaderResources(0, 1, &srv);
			break;
		default:
			ErrorLog::OutputToConsole("無効なタイプが使用されています");
			return false;
			break;
		}

		return true;
	}

	// ログ出力
	ErrorLog::OutputToConsole(std::string("SRV" + name + "が見つかりませんでした").c_str());
	return false;
}


// =============================
// UAV
// =============================
// 作成
//bool ResourceViewManager::CreateUAV(const std::string& name,
//	ID3D11Device* device,
//	ID3D11Resource* resource,
//	Format format,
//	UINT mipSlice)
//{
//	// 情報を構造体にまとめてマップに保存
//	auto uav = std::make_unique<UAVData>();
//
//	// バッファ作成
//	if (!uav->CreateUAV(
//		device,
//		resource,
//		DirectX_FormatConverter::ToDXFormat(format),
//		mipSlice))
//	{
//		ErrorLog::OutputToConsole(std::string(("UAV の作成失敗: " + name)).c_str());
//		return false;
//	}
//
//	// 配列に代入
//	m_UAVs[name] = std::move(uav);
//
//	// デバッグ用に名前を保存しておく
//	m_Logger.Log(("UAV : " + name).c_str());
//
//	return true;
//}

//// バインド
//bool ResourceViewManager::BindUAV(const std::string& name, ID3D11DeviceContext* context)
//{
//	// 探す
//	auto it = m_UAVs.find(name);
//
//	if (it != m_UAVs.end()) {
//
//		// 保存用 のちに配列対応
//		ID3D11UnorderedAccessView* uav = it->second.get()->GetUAV();
//
//		// バインド
//		context->CSSetUnorderedAccessViews(0, 1, &uav);
//
//		return true;
//	}
//
//	// ログ出力
//	ErrorLog::OutputToConsole(std::string("SRV" + name + "が見つかりませんでした").c_str());
//	return false;
//}


// =============================
// RTV
// =============================
// 作成
//bool ResourceViewManager::CreateRTV(const std::string& name,
//	ID3D11Device* device,
//	ID3D11Resource* resource,
//	Format format,
//	UINT mipSlice)
//{
//	// 情報を構造体にまとめてマップに保存
//	auto rtv = std::make_unique<RTVData>();
//
//	// バッファ作成
//	if (!rtv->CreateRTV(
//		device,
//		resource,
//		DirectX_FormatConverter::ToDXFormat(format),
//		mipSlice))
//	{
//		ErrorLog::OutputToConsole(std::string(("RTV の作成失敗: " + name)).c_str());
//		return false;
//	}
//
//	// 配列に代入
//	m_RTVs[name] = std::move(rtv);
//
//	// デバッグ用に名前を保存しておく
//	m_Logger.Log(("RTV : " + name).c_str());
//
//	return true;
//}

// バインド
//bool ResourceViewManager::BindRTV(const std::string& name, ID3D11DeviceContext* context, SETSHADERTYPE type)
//{
//	auto it = m_RTVs.find(name);
//
//	if (it != m_RTVs.end()) {
//		return it->second.get();
//
//		return true;
//	}
//
//	return false;
//}


// =============================
// DSV
// =============================
// 作成
//bool ResourceViewManager::CreateDSV(const std::string& name,
//	ID3D11Device* device,
//	ID3D11Resource* resource,
//	Format format,
//	UINT mipSlice)
//{
//	// 情報を構造体にまとめてマップに保存
//	auto dsv = std::make_unique<DSVData>();
//
//	// バッファ作成
//	if (!dsv->CreateDSV(
//		device,
//		resource,
//		DirectX_FormatConverter::ToDXFormat(format),
//		mipSlice))
//	{
//		ErrorLog::OutputToConsole(std::string(("DSV の作成失敗: " + name)).c_str());
//		return false;
//	}
//
//	// 配列に代入
//	m_DSVs[name] = std::move(dsv);
//
//	// デバッグ用に名前を保存しておく
//	m_Logger.Log(("DSV : " + name).c_str());
//
//	return true;
//}

// バインド
//bool ResourceViewManager::BindDSV(const std::string& name, ID3D11DeviceContext* context)
//{
//	auto it = m_DSVs.find(name);
//
//	if (it != m_DSVs.end()) {
//		return it->second.get();
//
//		return true;
//	}
//
//	return false;
//}


// View削除
void ResourceViewManager::ReleaseAllView()
{
	m_SRVs.clear();
	//m_UAVs.clear();
	//m_RTVs.clear();
	//m_DSVs.clear();
}
