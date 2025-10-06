
// ============================
// ヘッダー
// ============================
// 必須ヘッダー
#include "ResourceViewManager.h"
// Dataヘッダー
#include "ResourceViewData.h"
// 変換ヘッダー
#include "DirectX_BufferUtils.h"


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
		BufferUtils::toDXFormat(format),
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
SRVData* ResourceViewManager::GetFindSRV(const std::string& name)
{
	auto it = m_SRVs.find(name);

	if (it != m_SRVs.end()) {
		return it->second.get();
	}

	return nullptr;
}


// =============================
// UAV
// =============================
// 作成
bool ResourceViewManager::CreateUAV(const std::string& name,
	ID3D11Device* device,
	ID3D11Resource* resource,
	Format format,
	UINT mipSlice)
{
	// 情報を構造体にまとめてマップに保存
	auto uav = std::make_unique<UAVData>();

	// バッファ作成
	if (!uav->CreateUAV(
		device,
		resource,
		BufferUtils::toDXFormat(format),
		mipSlice))
	{
		ErrorLog::OutputToConsole(std::string(("UAV の作成失敗: " + name)).c_str());
		return false;
	}

	// 配列に代入
	m_UAVs[name] = std::move(uav);

	// デバッグ用に名前を保存しておく
	m_Logger.Log(("UAV : " + name).c_str());

	return true;
}

// ゲッター
UAVData* ResourceViewManager::GetFindUAV(const std::string& name)
{
	auto it = m_UAVs.find(name);

	if (it != m_UAVs.end()) {
		return it->second.get();
	}

	return nullptr;
}


// =============================
// RTV
// =============================
// 作成
bool ResourceViewManager::CreateRTV(const std::string& name,
	ID3D11Device* device,
	ID3D11Resource* resource,
	Format format,
	UINT mipSlice)
{
	// 情報を構造体にまとめてマップに保存
	auto rtv = std::make_unique<RTVData>();

	// バッファ作成
	if (!rtv->CreateRTV(
		device,
		resource,
		BufferUtils::toDXFormat(format),
		mipSlice))
	{
		ErrorLog::OutputToConsole(std::string(("RTV の作成失敗: " + name)).c_str());
		return false;
	}

	// 配列に代入
	m_RTVs[name] = std::move(rtv);

	// デバッグ用に名前を保存しておく
	m_Logger.Log(("RTV : " + name).c_str());

	return true;
}

// ゲッター
RTVData* ResourceViewManager::GetFindRTV(const std::string& name)
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
bool ResourceViewManager::CreateDSV(const std::string& name,
	ID3D11Device* device,
	ID3D11Resource* resource,
	Format format,
	UINT mipSlice)
{
	// 情報を構造体にまとめてマップに保存
	auto dsv = std::make_unique<DSVData>();

	// バッファ作成
	if (!dsv->CreateDSV(
		device,
		resource,
		BufferUtils::toDXFormat(format),
		mipSlice))
	{
		ErrorLog::OutputToConsole(std::string(("DSV の作成失敗: " + name)).c_str());
		return false;
	}

	// 配列に代入
	m_DSVs[name] = std::move(dsv);

	// デバッグ用に名前を保存しておく
	m_Logger.Log(("DSV : " + name).c_str());

	return true;
}

// ゲッター
DSVData* ResourceViewManager::GetFindDSV(const std::string& name)
{
	auto it = m_DSVs.find(name);

	if (it != m_DSVs.end()) {
		return it->second.get();
	}

	return nullptr;
}
