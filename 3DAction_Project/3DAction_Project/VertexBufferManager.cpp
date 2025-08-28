
// ======================================
// ヘッダー
// ======================================
// 必須ヘッダー
#include "VertexBufferManager.h"
// DirectXヘッダー
#include <d3d11.h>        // DirectXのAPIヘッダー
#include <wrl/client.h>   // スマートポインター
// ログ出力用ヘッダー
#include "ReportMessage.h"


// ======================================
// 静的メンバー配列
// ======================================
std::unordered_map<std::string, std::unique_ptr<VertexBufferData>> VertexBufferManager::m_VertexBuffers;


// ======================================
// 前方宣言
// ======================================
struct VertexBufferData
{
    // 頂点バッファ
    Microsoft::WRL::ComPtr<ID3D11Buffer> buffer = nullptr;
    // サイズ
    size_t size = 0;
    
    int striqde = 0;
    int slot = 0;
    int ofset = 0;
};


// =======================================
// 頂点バッファ作成
// =======================================
bool VertexBufferManager::CreateVertexBuffer(const std::string& name, size_t size, int slot, ID3D11Device* device)
{

}


// =======================================
// 頂点バッファを探して、戻り値で返す
// =======================================
ID3D11Buffer* VertexBufferManager::GetFindVertexBuffer(const std::string& name)
{

}


// =======================================
// 後処理
// =======================================
void VertexBufferManager::ReleaseAllVertexBuffers()
{
    m_VertexBuffers.clear();
}

