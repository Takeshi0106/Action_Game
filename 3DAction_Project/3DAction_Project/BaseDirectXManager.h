#pragma once
#include <filesystem>           // ファイルパスなど　C++17以降
#include <string>               // 文字列

// ==================================================================
// DirectXで使用するマネージャーの基底クラス
// シェーダー・テクスチャ・オブジェクトなどに派生させる
// ==================================================================

class BaseDirectXManager
{
protected:
	const  std::filesystem::path kFilePath;     // ファイルを参照しに行くパス
	const  std::filesystem::path kAssetLogPath; // アセットのログ デバッグビルド時に書出し、リリースビルド時にアセットがあるかを確認する 


#if defined(DEBUG) || defined(_DEBUG)
	std::filesystem::path m_DebugLogFilePath; // デバッグ用ログファイルパス　定数にできないため、使用注意
#endif

public:
	BaseDirectXManager(std::filesystem::path file, std::filesystem::path assetLog, std::filesystem::path debugPath)
		: kFilePath(file), kAssetLogPath(assetLog) 
	{
#if defined(DEBUG) || defined(_DEBUG)
		m_DebugLogFilePath = debugPath;
#endif
	}; // コンストラクタ

	~BaseDirectXManager() = default;                                                                                              // デストラクタ


};

