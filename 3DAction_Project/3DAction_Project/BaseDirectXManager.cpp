
// 必須ヘッダー
#include "BaseDirectXManager.h" // 自分のヘッダー

// ファイルパス用ヘッダー
#include <filesystem>  // ファイルパスなどを楽に扱える　C++17以降

// ファイル読込み、書出し
#include <fstream> // 外部ファイルとして書出し

// デバッグログやメッセージボックス出力用
#include "ReportMessage.h"


// 名前の配列を渡して、ログを書き出す
bool BaseDirectXManager::WriteLog()
{
	// フォルダがない場合作成
	if (!std::filesystem::exists(std::filesystem::path(kAssetLogPath).parent_path())) { // ファイルがない場合作成する
		if (!std::filesystem::create_directories(std::filesystem::path(kAssetLogPath).parent_path())) {
			ErrorLog::Log("使用したシェイダーログ : ログフォルダの作成に失敗しました");
			return false;
		}
	}

	std::ofstream ofs(kAssetLogPath, std::ios::binary | std::ios::out); // ファイルを開ける

	for (const std::string& name : m_Names) {
		ofs << name << "\n";                                              // ファイルに書き込み
	}
	ofs.close();                                                          // ファイルを閉じる

	return true;
}