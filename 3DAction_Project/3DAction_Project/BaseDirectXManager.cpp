
// ====================================================
// ヘッダー
// ====================================================
// 必須ヘッダー
#include "BaseDirectXManager.h" // 自分のヘッダー
// ファイルパス用ヘッダー
#include <filesystem>  // ファイルパスなどを楽に扱える　C++17以降
// ファイル読込み、書出し
#include <fstream> // 外部ファイルとして書出し
// デバッグログやメッセージボックス出力用
#include "ReportMessage.h"


// ===================================================
// 関数
// ===================================================
// 外部ファイルに使用したオブジェクト名を入れる
bool BaseDirectXManager::WriteLog()
{
	// フォルダがない場合作成
	if (!std::filesystem::exists(std::filesystem::path(kAssetLogPath).parent_path())) {
		if (!std::filesystem::create_directories(std::filesystem::path(kAssetLogPath).parent_path())) {
			ErrorLog::OutputToConsole("使用したシェイダーログ : ログフォルダの作成に失敗しました");
			return false;
		}
	}

	std::ofstream ofs(kAssetLogPath, std::ios::binary | std::ios::out); // ファイルを開ける

	ofs << m_UseObjectList; // 書き込む

	ofs.close(); // ファイルを閉じる

	return true;
}

// 名前を保存しておく
void BaseDirectXManager::Log(const char* name)
{
	m_UseObjectList += std::string(name) + "\n";
}