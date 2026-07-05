#pragma once

// =====================================================
// 【クラス概要】
// RT,SRV,UAVを動的に作成、管理などを行うマネージャ
// =====================================================


// =====================================================
// ヘッダー
// =====================================================
#include "ResourceViewData.h"
// スマートポインターヘッダー
#include <memory> // スマートポインター
// 配列のヘッダー
#include <unordered_map> // ハッシュ値検索
// バッファセッティング
#include "GraphicsEnums.h"
// シェーダータイプ
#include "UseShaderType.h"
// 文字列ヘッダー
#include "Hashed_String.h"


// =====================================================
// クラス
// =====================================================
class ResourceViewManager
{
private:
	// 配列
	std::unordered_map<Hashed_String, std::unique_ptr<SRVData>> m_SRVs;
	std::unordered_map<Hashed_String, std::unique_ptr<RTVData>> m_RTVs;
	std::unordered_map<Hashed_String, std::unique_ptr<DSVData>> m_DSVs;

public:
	ResourceViewManager() = default;
	~ResourceViewManager() = default;

	// 作成
	// SRV
	bool CreateSRV(
		const Hashed_String& name,
		ID3D11Device* device,
		ID3D11Texture2D* resource,
		Format format,
		UINT mostDetailedMip = 0,
		UINT mipLevels = -1);
	// RTV
	bool CreateRTV(
		const Hashed_String& name,
		ID3D11Device* device,
		ID3D11Texture2D* resource,
		UINT mmipSlice = 0);
	// DSV
	bool CreateDSV(
		const Hashed_String& name,
		ID3D11Device* device,
		ID3D11Texture2D* resource,
		Format format);

	// ゲッター  名前を入れて、返す
	bool BindSRV(const Hashed_String& name, ID3D11DeviceContext* context, SETSHADERTYPE type);

	// RTVとDSVのゲッター
	RTVData* GetRTV(const Hashed_String& name);
	DSVData* GetDSV(const Hashed_String& name);

	// View削除
	void ReleaseAllView();
};

