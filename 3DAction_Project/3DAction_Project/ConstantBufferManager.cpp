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
std::unordered_map<std::string, std::unique_ptr<ConstantBufferData>> ConstantBufferManager::m_ConstantBuffer;


// ======================================
// 前方宣言
// ======================================
struct ConstantBufferData { // 定数バッファデータ
	std::string name = "";                                         // デバッグ用に名前を保存
	Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer = nullptr; // 定数バッファ
	size_t size = 0;                                               // サイズ

	// コンストラクタ
	ConstantBufferData(const std::string& n, Microsoft::WRL::ComPtr<ID3D11Buffer> buf, size_t s)
		: name(n), constantBuffer(buf), size(s) {
	}
};


// =======================================
// 定数バッファ作成
// =======================================
bool ConstantBufferManager::CreateConstantBuffer(const std::string& name, size_t size, ID3D11Device* device)
{
	if (m_ConstantBuffer.count(name)) {
		return true; // 同じ名前があるときは戻る
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

	if (!ErrorLog::IsSuccessHRESULTWithOutputToConsole(hr, std::string(("定数バッファの作成失敗: " + name)).c_str())) { // 作成失敗
		return false;
	}

	// 情報を構造体にまとめてマップに保存
	auto data = std::make_unique<ConstantBufferData>(name, buffer, size);
	m_ConstantBuffer[name] = std::move(data);

	return true;
}


// =======================================
// 定数バッファを更新する
// =======================================
bool ConstantBufferManager::UpdateConstantBuffer(const std::string& name, const void* data, size_t dataSize, ID3D11DeviceContext* context)
{
	auto it = m_ConstantBuffer.find(name); // 定数バッファを探す
	if (it == m_ConstantBuffer.end()) { // 見つからないときの処理
		ErrorLog::Log(("定数バッファが見つかりません: " + name).c_str());
		return false;
	}

	if (dataSize > it->second->size) { // 定数バッファサイズを比較
		ErrorLog::Log(("データサイズがバッファサイズを超えています: " + name).c_str());
		return false;
	}

	D3D11_MAPPED_SUBRESOURCE mapped; // 定数バッファを更新するための、メモリ領域を受け取る
	HRESULT hr = context->Map(it->second->constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped); //CPUから定数バッファにアクセスできるようにする
	if (!ErrorLog::IsSuccessHRESULTWithOutputToConsole(hr, ("Map失敗: " + name).c_str())) { // アクセス失敗
		return false;
	}

	memcpy(mapped.pData, data, dataSize); // 更新したいデータをバッファにコピーする
	context->Unmap(it->second->constantBuffer.Get(), 0); // MAPを解除して、書き込めなくしている
	return true;
}


// =======================================
// 定数バッファを探して返す
// =======================================
ID3D11Buffer* ConstantBufferManager::GetFindConstantBuffer(const std::string& name)
{
	auto it = m_ConstantBuffer.find(name); // 探す
	if (it != m_ConstantBuffer.end()) { //　あったかどうかの確認
		return it->second->constantBuffer.Get(); // 定数バッファを返す
	}
	else {
		ErrorLog::Log(std::string(" 定数バッファ : " + name + " が見つかりませんでした").c_str());
	}

	return nullptr;
}
