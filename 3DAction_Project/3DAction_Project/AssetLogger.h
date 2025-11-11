#pragma once

// ==================================================================
// 【クラス概要】
// DirectXマネージャーの基底クラス
// デバッグ時に使用した名前を出力するように設定
// 
// 【責任】
// ログ出力
// リリース時は動かさないように空の関数に修正予定
// ==================================================================


// ==================================================================
// ヘッダー
// ==================================================================
#include <string> // 使用した情報を入れる　リリース時も使用する


// ==================================================================
// グローバル変数
// ==================================================================
namespace {
#if defined(DEBUG) || defined(_DEBUG)
	const std::string Type = "Debug";
#else
	const std::string Type = "Release";
#endif
}


// ==================================================================
// DirectXで使用するマネージャーの基底クラス
// シェーダー・テクスチャ・オブジェクトなどに派生させる
// ==================================================================
class AssetLogger
{
protected:
	// アセットのログ デバッグビルド時に書出し
	std::string kAssetLogPath = { "Debug/Log/" };
	// 使用したオブジェクトを入れる
	std::string m_UseObjectList;

public:

#if defined(DEBUG) || defined(_DEBUG)
	// コンストラクタ
	AssetLogger(const char* assetLog)
	{
		kAssetLogPath = std::string(kAssetLogPath + assetLog);
		m_UseObjectList += Type + "\n\n";
	}

	// 外部ファイルに使用したオブジェクトのリストを書き出す
	bool WriteLog();
	// オブジェクトの名前をセットする
	void Log(const char* name);

#else

	// コンストラクタ
	AssetLogger(const char* assetLog) {
		kAssetLogPath = std::string(kAssetLogPath + assetLog);
		m_UseObjectList += Type + "\n\n";
	}

	// 外部ファイルに使用したオブジェクトのリストを書き出す
	bool WriteLog();
	// オブジェクトの名前をセットする
	void Log(const char* name);
#endif

	// デストラクタ　
	~AssetLogger() = default;
};

