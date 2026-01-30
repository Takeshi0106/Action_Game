
// =======================================
// ヘッダー
// =======================================
// 必須ヘッダー
#include "IndexBufferManager.h"
// ログ出力用
#include "ReportMessage.h"
// バイナリーデータ
#include "BinaryView.h"


// =======================================
// バッファ作成
// =======================================
bool IndexBufferManager::CreateIndexBuffer(
	const Hashed_String& name,
    ID3D11Device* device,
	const uint32_t* indexData,
	uint32_t indexNumber)
{
    // 既に作成済み
    if (Exists(name)) {
        DebugLog::OutputToConsole(name.GetString() + 
            u8" インデックスバッファが既に作成されていました");
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
        ErrorLog::OutputToConsole(u8"インデックスバッファの作成に失敗");
        return false;
    }

    // バッファデータを配列に代入
    m_IndexBuffers[name] = std::move(vbd);


#if defined(DEBUG) || defined(_DEBUG)
    // コンソールに出力
    DebugLog::OutputToConsole(u8"インデックスバッファ " + 
        name.GetString() + u8" を作成しました");

    // 名前を設定
    m_IndexBuffers[name]->SetDebugName(
            reinterpret_cast<const char*>(
        name.GetString().GetBinaryView().GetData()));
#endif

    return true;
}


// =============================================
// インデックスバッファをバインド
// =============================================
uint32_t IndexBufferManager::BindIndexData(const Hashed_String& name,ID3D11DeviceContext* context) const
{
	// 探す
	auto it = m_IndexBuffers.find(name);

	if (it == m_IndexBuffers.end()) {
		WarningLog::OutputToConsole(u8" インデックスバッファ : " + 
            name.GetString() + u8" が見つかりませんでした");
        return UINT32_MAX;
	}

	// インデックスバッファをセット
	it->second->BindIndexBuffer(context);

    return it->second->GetIndexCount();
}


// =============================================
// インデックスバッファがあるかのチェック
// =============================================
bool IndexBufferManager::Exists(const Hashed_String& name) const
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
