
// ====================================================
// ヘッダー
// ====================================================
// 必須ヘッダー
#include "AssetLogger.h" // 自分のヘッダー
// 外部ファイルに書出し
#include "FileUtils.h"
// デバッグログやメッセージボックス出力用
#include "ReportMessage.h"


// ===================================================
// 関数
// ===================================================
// 外部ファイルに使用したオブジェクト名を入れる
bool AssetLogger::WriteLog()
{
	if (!FileUtis::WriteStringFile(kAssetLogPath, m_UseObjectList.GetBinaryView())) {
		ErrorLog::OutputToConsole(u8"ログファイルの書出しに失敗しました");
		return false;
	}

	return true;
}

// 名前を保存しておく
void AssetLogger::Log(const String& name)
{
	m_UseObjectList += name + u8"\n";
}
