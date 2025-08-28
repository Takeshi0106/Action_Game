
// ======================================
// ヘッダー
// ======================================
// 必須ヘッダー
#include "ConstantBufferManager.h" // 自分のヘッダー
// DirectXヘッダー
#include <d3d11.h>        // DirectXのAPIヘッダー
#include <wrl/client.h>   // スマートポインター
// ログ出力用ヘッダー
#include "ReportMessage.h"


// ======================================
// 静的メンバー配列
// ======================================
std::unordered_map<std::string, std::unique_ptr<ConstantBufferData>> ConstantBufferManager::m_ConstantBuffers;


// ======================================
// 前方宣言
// ======================================
struct ConstantBufferData { // 定数バッファデータ
	Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer = nullptr; // 定数バッファ
	size_t size = 0;                                               // サイズ
	int registerNumber = -1;                                       // スロット

	// コンストラクタ
	ConstantBufferData(Microsoft::WRL::ComPtr<ID3D11Buffer> buf, size_t _size, int slot)
		: constantBuffer(buf), size(_size), registerNumber(slot) {
	}
};


// =======================================
// 定数バッファ作成
// =======================================
bool ConstantBufferManager::CreateConstantBuffer(const std::string& name, size_t size, int slot, ID3D11Device* device)
{
	// エラーチェック
	if (m_ConstantBuffers.count(name)) 
	{
		// 定数バッファ取得
		auto& existing = m_ConstantBuffers[name];

		// サイズ比較
		if (existing->size != size) {
			ErrorLog::OutputToConsole(
				(name + " 同じ名前の定数バッファが作成されましたが、サイズが異なります").c_str());

			return false;
		}
		else {
			WarningLog::OutputToConsole((name + " 同じ名前の定数バッファが再作成されました").c_str());

			return true;
		}
	}

	D3D11_BUFFER_DESC desc{}; // 初期化
	desc.ByteWidth = static_cast<UINT>((size + 15) / 16 * 16); // 16バイト単位にする アライメント
	desc.Usage = D3D11_USAGE_DYNAMIC;                          // CPUから書き込み可能
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;               // 定数バッファで作成
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;              // CPUから書き込み可能
	desc.MiscFlags = 0;                                        // フラグなし
	desc.StructureByteStride = 0;                              // 構造体バッファではない

	// バッファ作成
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
	HRESULT hr = device->CreateBuffer(&desc, nullptr, buffer.GetAddressOf());

	if (FAILED(hr)) {
		ErrorLog::OutputToConsole(std::string(("定数バッファの作成失敗: " + name)).c_str());
		return false;
	}

	// 情報を構造体にまとめてマップに保存
	auto data = std::make_unique<ConstantBufferData>(buffer, size, slot);
	m_ConstantBuffers[name] = std::move(data);
	
	// デバッグ用に名前を保存しておく
	Log(name.c_str());

	return true;
}


// =======================================
// 定数バッファを探して返す
// =======================================
ID3D11Buffer* ConstantBufferManager::GetFindConstantBuffer(const std::string& name)
{
	// 探す
	auto it = m_ConstantBuffers.find(name); 

	if (it != m_ConstantBuffers.end()) 
	{
		// 定数バッファを返す
		return it->second->constantBuffer.Get();
	}
	else {
		ErrorLog::OutputToConsole(std::string(" 定数バッファ : " + name + " が見つかりませんでした").c_str());
	}

	return nullptr;
}


// =========================================
// 定数バッファをすべて削除
// =========================================
void ConstantBufferManager::ReleaseAllConstantBuffers()
{
	m_ConstantBuffers.clear();
}
