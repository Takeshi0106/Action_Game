
// ====================================================
// ヘッダー
// ====================================================
// 必須ヘッダー
#include "BaseDirectXManager.h" // 自分のヘッダー
// 外部ファイルに書出し
#include "FileUtils.h"
// デバッグログやメッセージボックス出力用
#include "ReportMessage.h"


// ===================================================
// 関数
// ===================================================
// 外部ファイルに使用したオブジェクト名を入れる
bool BaseDirectXManager::WriteLog()
{
	if (!FileUtis::WriteFile(kAssetLogPath, m_UseObjectList)) {
		ErrorLog::OutputToConsole("ログファイルの書出しに失敗しました");
		return false;
	}

	return true;
}

// 名前を保存しておく
void BaseDirectXManager::Log(const char* name)
{
	m_UseObjectList += std::string(name) + "\n";
}