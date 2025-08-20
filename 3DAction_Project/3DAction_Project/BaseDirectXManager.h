#pragma once

// ==================================================================
// 【クラス概要】
// DirectXマネージャーの基底クラス
// デバッグ時に使用した名前を出力するように設定
// ==================================================================


// ==================================================================
// ヘッダー
// ==================================================================
#if defined(DEBUG) || defined(_DEBUG)
#include <vector>  // デバッグ時にシェイダーの名前を保存しておく用
#include <string>  // オブジェクトの名前出力用
#endif


// ==================================================================
// DirectXで使用するマネージャーの基底クラス
// シェーダー・テクスチャ・オブジェクトなどに派生させる
// ==================================================================

class BaseDirectXManager
{
#if defined(DEBUG) || defined(_DEBUG)
protected:
	// アセットのログ デバッグビルド時に書出し
	const  char* kAssetLogPath;
	// 名前取得用
	std::vector<std::string> m_Names;


	// オブジェクトの名前を配列に入れて渡し、書き出すクラス
	bool WriteLog();
	// オブジェクトの名前をセットする
	void DebugSetName(const char* name);

public:
	// コンストラクタ
	BaseDirectXManager(const char* assetLog)
		: kAssetLogPath(assetLog) {
	}
	// デストラクタ　
	virtual ~BaseDirectXManager() { WriteLog(); }

#else
protected:
	// インラインでビルドしないように
	inline void DebugSetName(const char* name) {}

public:
	// 引き数をセットしない
	BaseDirectXManager(const char* assetLog) {} // コンストラクタ

	~BaseDirectXManager() = default;  // デストラクタ
#endif


};

