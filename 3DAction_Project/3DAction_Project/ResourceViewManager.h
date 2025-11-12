#pragma once

// =====================================================
// 【クラス概要】
// RT,SRV,UAVを動的に作成、管理などを行うマネージャ
// =====================================================


// =====================================================
// ヘッダー
// =====================================================
#include "ResourceViewData.h"
// 標準ヘッダー
#include <string>
// スマートポインターヘッダー
#include <memory> // スマートポインター
// 配列のヘッダー
#include <unordered_map> // ハッシュ値検索
// バッファセッティング
#include "GraphicsEnums.h"
// シェーダータイプ
#include "UseShaderType.h"
// アセットログ出力
#include "AssetLogger.h"


// =====================================================
// クラス
// =====================================================
class ResourceViewManager
{
private:
	// 配列
	std::unordered_map<std::string, std::unique_ptr<SRVData>> m_SRVs;
	//std::unordered_map<std::string, std::unique_ptr<UAVData>> m_UAVs;
	std::unordered_map<std::string, std::unique_ptr<RTVData>> m_RTVs;
	std::unordered_map<std::string, std::unique_ptr<DSVData>> m_DSVs;

	AssetLogger m_Logger{ "View.txt" };

public:
	ResourceViewManager() = default;
	~ResourceViewManager() { m_Logger.WriteLog(); }

	// 作成
	bool CreateSRV(const std::string& name,
		ID3D11Device* device,
		ID3D11Resource* resource,
		Format format,
		UINT mostDetailedMip = 0,
		UINT mipLevels = -1);

	//bool CreateUAV(const std::string& name,
	//	ID3D11Device* device,
	//	ID3D11Resource* resource,
	//	Format format,
	//	UINT mmipSlice = 0);

	//bool CreateRTV(const std::string& name,
	//	ID3D11Device* device,
	//	ID3D11Resource* resource,
	//	Format format,
	//	UINT mmipSlice = 0);

	//bool CreateDSV(const std::string& name,
	//	ID3D11Device* device,
	//	ID3D11Resource* resource,
	//	Format format,
	//	UINT mmipSlice = 0);

	// ゲッター  名前を入れて、返す
	bool BindSRV(const std::string& name, ID3D11DeviceContext* context, SETSHADERTYPE type);
	//bool BindUAV(const std::string& name, ID3D11DeviceContext* context);
	//bool BindRTV(const std::string& name, ID3D11DeviceContext* context, SETSHADERTYPE type);
	//bool BindDSV(const std::string& name, ID3D11DeviceContext* context, SETSHADERTYPE type);

	// View削除
	void ReleaseAllView();
};

