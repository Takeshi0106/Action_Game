
// =======================================
// ヘッダー
// =======================================
// 必須ヘッダー
#include "IndexBufferManager.h"
// ログ出力用
#include "ReportMessage.h"


// =======================================
// バッファ作成
// =======================================
bool IndexBufferManager::CreateIndexBuffer(
	std::string name,
    ID3D11Device* device,
	int* indexData,
	int indexNumber)
{
    // 既に作成済み
    if (Exists(name)) {
        DebugLog::OutputToConsole((name + " インデックスバッファが既に作成されていました").c_str());
        return true;
    }

    // 領域確保
    auto vbd = std::make_unique<IndexBufferData>();

    // バッファ作成呼び出し
    if (!vbd->CreateIndexBuffer(
        device,
        indexData,
        indexNumber))
    {
        ErrorLog::OutputToConsole("インデックスバッファの作成に失敗");
        return false;
    }

    // バッファデータを配列に代入
    m_IndexBuffers[name] = std::move(vbd);

    // バッファの名前を保存
    m_Logger.Log(name.c_str());

    // コンソールに出力
    DebugLog::OutputToConsole((name + " を作成しました").c_str());

    return true;
}


// =============================================
// インデックスバッファを探す
// =============================================
IndexBufferData* IndexBufferManager::GetFindIndexData(const std::string& name) const
{
	// 探す
	auto it = m_IndexBuffers.find(name);

	if (it != m_IndexBuffers.end()) {
		// 頂点データを返す
		return it->second.get();
	}
	else {
		WarningLog::OutputToConsole(std::string(" インデックスバッファ : " + name + " が見つかりませんでした").c_str());
	}

	return nullptr;
}


// =============================================
// インデックスバッファがあるかのチェック
// =============================================
bool IndexBufferManager::Exists(const std::string& name) const
{
	return m_IndexBuffers.find(name) != m_IndexBuffers.end();
}


// =============================================
// 後処理
// =============================================
void IndexBufferManager::ReleaseAllIndexBuffers()
{
	m_IndexBuffers.clear();
}
