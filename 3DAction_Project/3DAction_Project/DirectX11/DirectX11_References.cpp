
// ==============================================
// ヘッダー
// ==============================================
// 必須ヘッダー
#include "DirectX11_References.h"
// リフレクション用ヘッダー
#include <d3d11shader.h>
#include <d3dcompiler.h>
// DirectX用ヘッダー
#include <dxgiformat.h>
// DirectX用スマートポインター
#include <wrl/client.h>
// 文字列ヘッダー
#include <string>
// 配列ヘッダー
#include <vector>
// ログ出力
#include "../ReportMessage.h"


// ==============================================
// 構造体
// ==============================================
// 定数バッファ情報
struct DX11_CBInfo
{
	// 定数バッファの名前
	std::string m_Name = "";
	// レジスタ番号
	uint16_t m_RegisterNumber = 0;
	// 必ず１６の倍数にする
	size_t m_Size = 0;
};

// 入力レイアウト情報
struct DX11_ILInfo {
	// セマンティックの名前
	std::string m_SemanticName = "";
	// セマンティックの番号
	uint16_t m_SemanticIndex = 0;
	// スロット番号
	uint16_t m_InputSlot = 0;
	// データの形式 (DXGI)
	uint16_t m_Format = 0;
};


// ==============================================
// 前方宣言
// ==============================================
// 解析情報を引数の配列に入れる関数
bool ShaderInfoAcquisition(
	const void* _blob,
	size_t size,
	std::vector<DX11_CBInfo> _cbInfo,
	std::vector<DX11_ILInfo> _ilInfo);


// ==============================================
// シェーダーリファレンス出力
// ==============================================
void DirectX11_References::OutputShaderReferences(const std::string _shaderPath, const void* _blob, const size_t size)
{
	// 取得する情報を格納する配列
	std::vector<DX11_CBInfo> cbInfos;
	std::vector<DX11_ILInfo> ilInfos;

	// リファレンス
	if (!ShaderInfoAcquisition(_blob, size, cbInfos, ilInfos)) {
		ErrorLog::OutputToMessageBox("シェーダーリファレンスに失敗しました");
		return;
	}

	// 外部ファイルに書き出し

}


// ==============================================
// シェーダー情報取得関数
// ==============================================
bool ShaderInfoAcquisition(
	const void* _blob, 
	size_t _size,
	std::vector<DX11_CBInfo> _cbInfo,
	std::vector<DX11_ILInfo> _ilInfo)
{
	// シェーダーリフレクターのスマートポインター
	Microsoft::WRL::ComPtr<ID3D11ShaderReflection> reflector;

	// バイナリーデータを解析
	HRESULT hr = D3DReflect(_blob, _size, IID_PPV_ARGS(&reflector));
	if (FAILED(hr)) {
		ErrorLog::OutputToConsole("リファレンス失敗 :" + hr);
		return false;
	}

	// 定数バッファの情報を取得
	D3D11_SHADER_DESC shaderDesc = {};
	reflector->GetDesc(&shaderDesc);

	// 配列のサイズを設定
	_cbInfo.resize(shaderDesc.ConstantBuffers);
	_ilInfo.resize(shaderDesc.InputParameters);

	// シェーダーステージを文字列で取得
	std::string creator = shaderDesc.Creator;

	// 入力レイアウトを取得する
	for (int k = 0; k < int(shaderDesc.InputParameters); k++)
	{
		// 入力情報を取得
		D3D11_SIGNATURE_PARAMETER_DESC paramDesc = {};
		reflector->GetInputParameterDesc(k, &paramDesc);

		_ilInfo[k].m_SemanticName = paramDesc.SemanticName;
		_ilInfo[k].m_SemanticIndex = (int)paramDesc.SemanticIndex;
		_ilInfo[k].m_InputSlot = 0;

		// ComponentMask から DXGI_FORMAT を推定
		if (paramDesc.Mask == 1) {
			_ilInfo[k].m_Format = (int)DXGI_FORMAT_R32_FLOAT;
		}
		else if (paramDesc.Mask <= 3) {
			_ilInfo[k].m_Format = (int)DXGI_FORMAT_R32G32_FLOAT;
		}
		else if (paramDesc.Mask <= 7) {
			_ilInfo[k].m_Format = (int)DXGI_FORMAT_R32G32B32_FLOAT;
		}
		else if (paramDesc.Mask <= 15) {
			_ilInfo[k].m_Format = (int)DXGI_FORMAT_R32G32B32A32_FLOAT;
		}
		else {
			ErrorLog::OutputToConsole("未知のフォーマットです");
			return false;
		}
	}


	// シェーダー内の定数バッファの数だけループする
	for (int i = 0; i < (int)shaderDesc.ConstantBuffers; i++)
	{
		// i番目の定数バッファ情報を取得
		ID3D11ShaderReflectionConstantBuffer* cb = reflector->GetConstantBufferByIndex(i);

		// 定数バッファの名前とサイズを取得
		D3D11_SHADER_BUFFER_DESC bufferDesc = {};
		cb->GetDesc(&bufferDesc);

		// 名前が付けられていない定数バッファがあれば、プロジェクトを停止させる
		if (bufferDesc.Name == nullptr || std::strlen(bufferDesc.Name) == 0) {
			ErrorLog::OutputToConsole("定数バッファの情報に名前が入っていませんでした");
			return false;
		}

		// nullptrチェックをしてるため問題ないが警告が出るため、？演算子を使用してstring 型に代入
		std::string bufferName = bufferDesc.Name ? bufferDesc.Name : "";


		// --------------------------------------------------------------------------------------------
		// レジスタ番号と、サイズは一緒に取得できないため、同じ名前のバインド情報を探して、取得する
		// --------------------------------------------------------------------------------------------
		int registerNumber = -1;

		// シェーダー内のバインド可能なリーソース分ループさせる（テクスチャやサンプラーなど）
		for (int b = 0; b < int(shaderDesc.BoundResources); b++)
		{
			// バインド情報を取得 (レジスタ番号や名前、種類など)
			D3D11_SHADER_INPUT_BIND_DESC bindDesc = {};
			reflector->GetResourceBindingDesc(b, &bindDesc);

			// バインド情報が定数バッファかのチェック
			if (bindDesc.Type != D3D_SIT_CBUFFER) { continue; }

			// 名前が使われていない定数バッファがあれば、プロジェクトを停止させる
			if (bindDesc.Name == nullptr || std::strlen(bindDesc.Name) == 0) {
				ErrorLog::OutputToConsole("リフレクションした情報に名前が入っていませんでした");
				return false;
			}

			// nullptrチェックをしてるため問題ないが警告が出るため、？演算子を使用してstring 型に代入
			std::string bindName = bindDesc.Name ? bindDesc.Name : "";

			// 同じ名前かをチェック
			if (bindName == bufferName) {
				registerNumber = bindDesc.BindPoint; // レジスタ番号を代入
				break;
			}
		}

		// 配列に情報を代入する
		if (registerNumber != -1) {
			_cbInfo[i].m_Name = bufferName;
			_cbInfo[i].m_RegisterNumber = registerNumber;
			_cbInfo[i].m_Size = bufferDesc.Size;
		}
		else {
			ErrorLog::OutputToConsole("バインド番号が見つかりませんでした");
			return false;
		}
	}

	return true;
}
