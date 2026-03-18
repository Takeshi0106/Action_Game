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
#include "UTF8_String.h"


// ==================================================================
// グローバル変数
// ==================================================================
namespace {
#if defined(DEBUG) || defined(_DEBUG)
	const String Type = u8"Debug";
#else
	const String Type = u8"Release";
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
	String kAssetLogPath = u8"Debug/Log/";
	// 使用したオブジェクトを入れる
	String m_UseObjectList;

public:

#if defined(DEBUG) || defined(_DEBUG)
	// コンストラクタ
	AssetLogger(const String& assetLog)
	{
		kAssetLogPath += assetLog;
		m_UseObjectList += Type + u8"\n\n";
	}

	// 外部ファイルに使用したオブジェクトのリストを書き出す
	bool WriteLog();
	// オブジェクトの名前をセットする
	void Log(const String& name);

#else

	// コンストラクタ
	AssetLogger(const String& assetLog) {
		kAssetLogPath += assetLog;
		m_UseObjectList += Type + u8"\n\n";
	}

	// 外部ファイルに使用したオブジェクトのリストを書き出す
	bool WriteLog();
	// オブジェクトの名前をセットする
	void Log(const String& name);
#endif

	// デストラクタ　
	~AssetLogger() = default;
};

