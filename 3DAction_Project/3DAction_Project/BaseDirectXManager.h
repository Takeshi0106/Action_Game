#pragma once

// 標準ヘッダー
#include <string> // 文字列

#if defined(DEBUG) || defined(_DEBUG)
#include <vector>  // デバッグ時にシェイダーの名前を保存しておく用
#endif

// ==================================================================
// DirectXで使用するマネージャーの基底クラス
// シェーダー・テクスチャ・オブジェクトなどに派生させる
// ==================================================================

class BaseDirectXManager
{
protected:
	const  std::string kFilePath;     // ファイルを参照しに行くパス
	const  std::string kAssetLogPath; // アセットのログ デバッグビルド時に書出し、リリースビルド時にアセットがあるかを確認する 

#if defined(DEBUG) || defined(_DEBUG)
	std::vector<std::string> m_Names; // 名前取得用
	bool WriteLog(); // オブジェクトの名前を配列に入れて渡し、書き出すクラス
#endif

public:
	BaseDirectXManager(std::string file, std::string assetLog)
		: kFilePath(file), kAssetLogPath(assetLog) {
	}; // コンストラクタ

	~BaseDirectXManager() = default;  // デストラクタ


};

