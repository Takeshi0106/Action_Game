#pragma once

// =====================================================
// 【クラス概要】
// RT,SRV,UAVを動的に作成、管理などを行うマネージャ
// =====================================================


// =====================================================
// ヘッダー
// =====================================================
// 基底クラスヘッダー
#include "BaseDirectXManager.h"
// 標準ヘッダー
#include <string>
// スマートポインターヘッダー
#include <memory> // スマートポインター
// 配列のヘッダー
#include <unordered_map> // ハッシュ値検索


// =====================================================
// 前方宣言
// =====================================================
class SRVData;
class UAVData;
class RTVData;
class DSVData;

// =====================================================
// クラス
// =====================================================
class ResourceViewManager : public BaseDirectXManager
{
private:
	// 配列
	static std::unordered_map<std::string, std::unique_ptr<SRVData>> m_SRVs;
	static std::unordered_map<std::string, std::unique_ptr<UAVData>> m_UAVs;
	static std::unordered_map<std::string, std::unique_ptr<RTVData>> m_RTVs;
	static std::unordered_map<std::string, std::unique_ptr<DSVData>> m_DSVs;

public:
	// 作成


	// ゲッター  名前を入れて、返す
	static SRVData* GetFindSRV(const std::string& name);
	static UAVData* GetFindUAV(const std::string& name);
	static RTVData* GetFindRTV(const std::string& name);
	static DSVData* GetFindDSV(const std::string& name);
};

