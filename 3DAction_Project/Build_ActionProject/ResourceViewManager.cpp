
// ============================
// ヘッダー
// ============================
// 必須ヘッダー
#include "ResourceViewManager.h"
// Dataヘッダー
#include "ResourceViewData.h"
// 変換ヘッダー
#include "DirectX11_FormatConverter.h"
// ログ出力
#include "ReportMessage.h"


// =============================
// SRV
// =============================
// 作成
bool ResourceViewManager::CreateSRV(
	const Hashed_String& name,
	ID3D11Device* device,
	ID3D11Texture2D* resource,
	Format format,
	UINT mostDetailedMip,
	UINT mipLevels)
{
	// 同じ名前のものが存在するか確認
	if (m_SRVs.find(name) != m_SRVs.end())
	{
		ErrorLog::OutputToConsole(u8"同じ名前のSRVが存在します: " + name.GetString());
		return false;
	}

	// 情報を構造体にまとめてマップに保存
	auto srv = std::make_unique<SRVData>();

	// バッファ作成
	if (!srv->CreateSRV(
		device,
		resource,
		DirectX11_FormatConverter::ToDXFormat(format),
		mostDetailedMip,
		mipLevels))
	{
		ErrorLog::OutputToConsole(u8"SRV の作成失敗: " + name.GetString());
		return false;
	}

	// 配列に代入
	m_SRVs[name] = std::move(srv);

	// デバッグ用に名前を保存しておく
	m_Logger.Log(u8"SRV : " + name.GetString());

	return true;
}

// ゲッター
bool ResourceViewManager::BindSRV(
	const Hashed_String& name, 
	ID3D11DeviceContext* context, 
	SETSHADERTYPE type)
{
	// 探す
	auto it = m_SRVs.find(name);

	if (it != m_SRVs.end()) {
		
		// 保存用 のちに配列対応
		ID3D11ShaderResourceView* srv = it->second.get()->GetSRV();

		// バインド
		switch (type)
		{
		case SETSHADERTYPE::VERTEXSHADER:
			context->VSSetShaderResources(0, 1, &srv);
			break;
		case SETSHADERTYPE::PIXSELSHADER:
			context->PSSetShaderResources(0, 1, &srv);
			break;
		case SETSHADERTYPE::CONPUTESHADER:
			context->CSSetShaderResources(0, 1, &srv);
			break;
		default:
			ErrorLog::OutputToConsole(u8"無効なタイプが使用されています");
			return false;
			break;
		}

		return true;
	}

	// ログ出力
	ErrorLog::OutputToConsole(u8"SRV" + name.GetString() + u8"が見つかりませんでした");
	return false;
}

// =============================
// RTV
// =============================
// 作成
bool ResourceViewManager::CreateRTV(
	const Hashed_String& name,
	ID3D11Device* device,
	ID3D11Texture2D* resource,
	UINT mipSlice)
{
	// 同じ名前のものが存在するか確認
	if (m_RTVs.find(name) != m_RTVs.end())
	{
		ErrorLog::OutputToConsole(u8"同じ名前のRTVが存在します: " + name.GetString());
		return false;
	}

	// 情報を構造体にまとめてマップに保存
	auto rtv = std::make_unique<RTVData>();

	// バッファ作成
	if (!rtv->CreateRTV(
		device,
		resource,
		mipSlice))
	{
		ErrorLog::OutputToConsole(u8"RTV の作成失敗: " + name.GetString());
		return false;
	}

	// 配列に代入
	m_RTVs[name] = std::move(rtv);

	// デバッグ用に名前を保存しておく
	m_Logger.Log(u8"RTV : " + name.GetString());

	return true;
}

// バインド
RTVData* ResourceViewManager::GetRTV(
	const Hashed_String& name)
{
	auto it = m_RTVs.find(name);

	if (it != m_RTVs.end()) {
		return it->second.get();
	}

	return nullptr;
}

// =============================
// DSV
// =============================
// 作成
bool ResourceViewManager::CreateDSV(
	const Hashed_String& name,
	ID3D11Device* device,
	ID3D11Texture2D* resource,
	Format format)
{
	// 同じ名前のものが存在するか確認
	if (m_DSVs.find(name) != m_DSVs.end())
	{
		ErrorLog::OutputToConsole(u8"同じ名前のDSVが存在します: " + name.GetString());
		return false;
	}

	// 情報を構造体にまとめてマップに保存
	auto dsv = std::make_unique<DSVData>();

	// バッファ作成
	if (!dsv->CreateDSV(
		device,
		resource,
		DirectX11_FormatConverter::ToDXFormat(format)))
	{
		ErrorLog::OutputToConsole(u8"DSV の作成失敗: " + name.GetString());
		return false;
	}

	// 配列に代入
	m_DSVs[name] = std::move(dsv);

	// デバッグ用に名前を保存しておく
	m_Logger.Log(u8"DSV : " + name.GetString());

	return true;
}

// バインド
DSVData* ResourceViewManager::GetDSV(const Hashed_String& name)
{
	auto it = m_DSVs.find(name);

	if (it != m_DSVs.end()) {
		return it->second.get();
	}

	return nullptr;
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


// View削除
void ResourceViewManager::ReleaseAllView()
{
	m_SRVs.clear();
	m_RTVs.clear();
	m_DSVs.clear();

	//m_UAVs.clear();
}
