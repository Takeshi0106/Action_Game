
// ======================================
// ヘッダー
// ======================================
// 必須ヘッダー
#include "ConstantBufferManager.h" // 自分のヘッダー
// 自作列挙型をDirectX用に変換
#include "DirectX_BufferUtils.h"
// ログ出力用ヘッダー
#include "ReportMessage.h"


// ======================================
// 静的メンバー配列
// ======================================
// std::unordered_map<std::string, std::unique_ptr<ConstantBufferData>> ConstantBufferManager::m_ConstantBuffers;


// ========================================
// 定数バッファ作成
// ========================================
bool ConstantBufferManager::CreateConstantBuffer(const std::string& constantName,
	ID3D11Device* device,
	const void* data,
	size_t size,
	BufferUsage usage,
	CPUAccess access)
{
	// エラーチェック
	if (m_ConstantBuffers.count(constantName))
	{
		// 定数バッファ取得
		auto& existing = m_ConstantBuffers[constantName];

		// サイズ比較
		if (existing->GetSize() != size) {
			ErrorLog::OutputToConsole(
				(constantName + " 同じ名前の定数バッファが作成されましたが、サイズが異なります").c_str());

			return false;
		}
		else {
			WarningLog::OutputToConsole((constantName + " 同じ名前の定数バッファが再作成されました").c_str());

			return true;
		}
	}

	// 情報を構造体にまとめてマップに保存
	auto bafferData = std::make_unique<ConstantBufferData>();
	
	// バッファ作成
	if (!bafferData->CreateConstantBuffer(
		device,
		data,
		size,
		BufferUtils::ToDXUsage(usage),
		D3D11_CPU_ACCESS_FLAG(BufferUtils::ToDXCPUAccess(access))))
	{
		ErrorLog::OutputToConsole(std::string(("定数バッファの作成失敗: " + constantName)).c_str());
		return false;
	}
	
	// 配列に代入
	m_ConstantBuffers[constantName] = std::move(bafferData);

	// デバッグ用に名前を保存しておく
	m_Logger.Log(constantName.c_str());

	return true;
}


// =======================================
// 定数バッファを探して返す
// =======================================
ConstantBufferData* ConstantBufferManager::GetFindConstantBuffer(const std::string& name)
{
	// 探す
	auto it = m_ConstantBuffers.find(name); 

	if (it != m_ConstantBuffers.end()) 
	{
		// 定数バッファを返す
		return it->second.get();
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
