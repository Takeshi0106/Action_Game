#pragma once
#include <filesystem>           // ファイルパスなど　C++17以降
#include <string>               // 文字列
#include <unordered_map>        // ハッシュ値配列

// ==================================================================
// DirectXで使用するマネージャーの基底クラス
// シェーダー・テクスチャ・オブジェクトなどに派生させる
// ==================================================================

class BaseDirectXManager
{
protected:
	const  std::filesystem::path kFilePath;     // ファイルを参照しに行くパス
	const  std::filesystem::path kAssetLogPath; // アセットのログ デバッグビルド時に書出し、リリースビルド時にアセットがあるかを確認する 

public:
	BaseDirectXManager(std::filesystem::path file, std::filesystem::path assetLog) : kFilePath(file), kAssetLogPath(assetLog) {}; // コンストラクタ
	~BaseDirectXManager() = default;                                   // デストラクタ


};

