#pragma once

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
protected:
	const  char* kFilePath;     // ファイルを参照しに行くパス
	const  char* kAssetLogPath; // アセットのログ デバッグビルド時に書出し、リリースビルド時にアセットがあるかを確認する 

#if defined(DEBUG) || defined(_DEBUG)
	std::vector<std::string> m_Names; // 名前取得用
	bool WriteLog(); // オブジェクトの名前を配列に入れて渡し、書き出すクラス
#endif

public:
	BaseDirectXManager(const char* file, const char* assetLog)
		: kFilePath(file), kAssetLogPath(assetLog) {
	}; // コンストラクタ

	~BaseDirectXManager() = default;  // デストラクタ


};

