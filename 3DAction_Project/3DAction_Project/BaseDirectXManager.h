#pragma once

// 標準ヘッダー
#include <string> // 文字列

// ==================================================================
// DirectXで使用するマネージャーの基底クラス
// シェーダー・テクスチャ・オブジェクトなどに派生させる
// ==================================================================

class BaseDirectXManager
{
protected:
	const  std::string kFilePath;     // ファイルを参照しに行くパス
	const  std::string kAssetLogPath; // アセットのログ デバッグビルド時に書出し、リリースビルド時にアセットがあるかを確認する 

public:
	BaseDirectXManager(std::string file, std::string assetLog)
		: kFilePath(file), kAssetLogPath(assetLog) {
	}; // コンストラクタ

	~BaseDirectXManager() = default;  // デストラクタ


};

