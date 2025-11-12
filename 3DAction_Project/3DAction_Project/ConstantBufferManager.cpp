
// ======================================
// ヘッダー
// ======================================
// 必須ヘッダー
#include "ConstantBufferManager.h" // 自分のヘッダー
// 自作列挙型をDirectX用に変換
#include "DirectX_FormatConverter.h"
// ログ出力用ヘッダー
#include "ReportMessage.h"


// ========================================
// 定数バッファ作成
// ========================================
bool ConstantBufferManager::CreateConstantBuffer(
	const std::string constantName,
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
		DirectX_FormatConverter::ToDXUsage(usage),
		D3D11_CPU_ACCESS_FLAG(DirectX_FormatConverter::ToDXCPUAccess(access))))
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
// 定数バッファ更新
// =======================================
bool ConstantBufferManager::UpdateConstantBuffer(const std::string& name, ID3D11DeviceContext* context, const void* data, int size)
{
	// 探す
	auto it = m_ConstantBuffers.find(name);

	// あるかどうかのチェック
	if (it != m_ConstantBuffers.end())
	{
		it->second.get()->UpdateConstantBuffer(context, data, size);
		return true;
	}

	ErrorLog::OutputToConsole("定数バッファが見つかりませんでした");
	return false;
}


// =======================================
// 定数バッファを探して,バインドを行う
// =======================================
bool ConstantBufferManager::BindConstantBuffer(const std::vector<ConstantBufferInfo>* cbInfo, ID3D11DeviceContext* context, SETSHADERTYPE type)
{
	// バッファ配列を作成
	std::vector<ID3D11Buffer*> buffers(cbInfo->size(), nullptr);

	// 頂点シェーダーの定数バッファバインド設定
	for (size_t i = 0; i < cbInfo->size(); i++)
	{
		// 探す
		auto it = m_ConstantBuffers.find((*cbInfo)[i].GetName());

		// あるかどうかのチェック
		if (it != m_ConstantBuffers.end())
		{
			// バッファに代入
			buffers[i] = it->second.get()->GetBuffer();
		}
		else {
			ErrorLog::OutputToConsole(std::string(" 定数バッファ : " + (*cbInfo)[i].GetName() + " が見つかりませんでした").c_str());
			return false;
		}
	}

	// 定数バッファをまとめてバインド
	switch (type)
	{
	case VERTEXSHADER:
		context->VSSetConstantBuffers(
			0,
			static_cast<UINT>(buffers.size()),
			buffers.data());
		break;
	case PIXSELSHADER:
		context->PSSetConstantBuffers(
			0,
			static_cast<UINT>(buffers.size()),
			buffers.data());
		break;
	case CONPUTESHADER:
		context->CSSetConstantBuffers(
			0,
			static_cast<UINT>(buffers.size()),
			buffers.data());
		break;

	default:
		ErrorLog::OutputToConsole(("無効なシェーダータイプが選択されています。"));
		break;
	}

	return true;
}


// =========================================
// 定数バッファをすべて削除
// =========================================
void ConstantBufferManager::ReleaseAllConstantBuffers()
{
	m_ConstantBuffers.clear();
}
