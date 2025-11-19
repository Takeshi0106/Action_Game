
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
	const uint32_t* indexData,
	uint32_t indexNumber)
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


#if defined(DEBUG) || defined(_DEBUG)
    // コンソールに出力
    DebugLog::OutputToConsole(("インデックスバッファ " + name + " を作成しました").c_str());

    // 名前を設定
    m_IndexBuffers[name]->GetBuffer()->SetPrivateData(
        WKPDID_D3DDebugObjectName,
        UINT(name.size()),
        name.c_str());
#endif

    return true;
}


// =============================================
// インデックスバッファをバインド
// =============================================
uint32_t IndexBufferManager::BindIndexData(const std::string& name,ID3D11DeviceContext* context) const
{
	// 探す
	auto it = m_IndexBuffers.find(name);

	if (it == m_IndexBuffers.end()) {
		WarningLog::OutputToConsole(std::string(" インデックスバッファ : " + name + " が見つかりませんでした").c_str());
        return UINT32_MAX;
	}

	// インデックスバッファをセット
    context->IASetIndexBuffer(
        it->second->GetBuffer(),
        DXGI_FORMAT_R32_UINT,
		0);

    return it->second->GetIndexCount();
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
