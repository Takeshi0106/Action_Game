
// ===========================================
// ヘッダー
// ===========================================
// 必須ヘッダー
#include "ShaderReflection.h"
// リフレクション用ヘッダー
#include <d3d11shader.h>
#include <d3dcompiler.h>
// DirectX用ヘッダー
#include <dxgiformat.h>
// ファイル出力、読込み用ヘッダー
#include <filesystem>
#include <fstream>                      // 外部ファイルとして書出し・読み出し
// DirectX用スマートポインター
#include <wrl/client.h>  // DirectX用のスマートポインター
// ログ出力
#include "ReportMessage.h"
// セーブロード用
#include "SaveLoadUtils.h"


// ================================================
// グローバル変数
// ================================================
namespace {

	// リファレンス情報書き込み、読込み用
	// シェーダー関連
	const std::string kShaderStart = "Shader"; // シェーダーの数

}


// ==================================================
// プロトタイプ宣言
// ==================================================

// 読み込む関数
// 外部ファイルを引き数のstring に代入する関数
bool LoadFile(const char* path, std::string& outContent);
// Stringを渡して、配列に代入する処理
bool ParseShaderInfo(const std::string_view& dataView, std::vector<ShaderInfo>& outShaderInfo);


// =================================================
// 読込み関数
// =================================================
bool ShaderInfoInput(const char* kShader_ConstantInfoPath, std::vector<ShaderInfo>& loadAllShaderInfo)
{
	// ファイルの内容を全て入れる変数
	std::string allShaderInfo;

	// ファイルパスの情報をStringに入れる処理
	if (!LoadFile(kShader_ConstantInfoPath, allShaderInfo)) {
		ErrorLog::OutputToConsole("リフレクション情報ロード失敗");
		return false;
	}

	// allShaderInfo を参照する変数
	std::string_view dataView(allShaderInfo);

	if (!ParseShaderInfo(allShaderInfo, loadAllShaderInfo)) {
		ErrorLog::OutputToConsole("リフレクション情報の読み込みに失敗");
		return false;
	}

	return true;
}


// ==============================================
// 引数のStringに取得した情報を入れる処理
// ==============================================
bool LoadFile(const char* path, std::string& outContent)
{
	// ファイルを開ける
	std::ifstream ifs(path, std::ios::in);
	if (!ifs.is_open()) {
		ErrorLog::OutputToConsole((std::string(path) + " ファイルのオープンに失敗しました").c_str());
		return false;
	}

	// ファイルサイズを取得
	
	// 末尾に移動
	ifs.seekg(0, std::ios::end);
	// 全体のサイズを取得
	size_t fileSize = static_cast<size_t>(ifs.tellg());
	// 先頭に戻す
	ifs.seekg(0, std::ios::beg);

	// ファイル内容を格納する文字列を確保
	outContent.resize(fileSize);

	// 代入する
	ifs.read(outContent.data(), fileSize);

	// ファイルを閉じる
	ifs.close();

	return true;
}


// ================================================
// String_viewを渡して、配列に代入する処理
// ================================================
bool ParseShaderInfo(const std::string_view& dataView, std::vector<ShaderInfo>& loadShaderInfo)
{
	std::string data(dataView);
	std::vector<std::string_view> blocks;

	// ブロックごとに分ける
	if (!LoadUtils::ExtractSubBlocks(data, kShaderStart, blocks)) {
		ErrorLog::OutputToConsole("シェーダー情報をブロックに出来ませんでした");
		return false;
	}

	// リサイズする
	loadShaderInfo.resize(blocks.size());

	// シェーダーに情報を入れていく
	for (int i = 0; i < (int)blocks.size(); i++)
	{
		if (!loadShaderInfo[i].Deserialize(std::string(blocks[i]))) {
			ErrorLog::OutputToConsole("シェーダー情報を読み込むことが出来ませんでした");
			return false;
		}
	}

	return true;
}


#if defined(DEBUG) || defined(_DEBUG)
// ============================================
// リフレクション関数
// ============================================
bool Reflect(void* blob, size_t blobSize, std::vector<ConstantBufferInfo>& CBInfo, std::vector<InputLayoutInfo>& ILinfo)
{
	/// リフレクション作成
	Microsoft::WRL::ComPtr<ID3D11ShaderReflection> reflector;

	// バイナリーデータを解析
	HRESULT hr = D3DReflect(blob, blobSize, IID_PPV_ARGS(&reflector));
	if (FAILED(hr)) {
		ErrorLog::OutputToConsole("リファレンス失敗 :" + hr);
		return false;
	}

	// 定数バッファの情報を取得
	D3D11_SHADER_DESC shaderDesc = {};
	reflector->GetDesc(&shaderDesc);

	// 配列のサイズを設定
	CBInfo.resize(shaderDesc.ConstantBuffers);
	ILinfo.resize(shaderDesc.InputParameters);

	// シェーダーステージを文字列で取得
	std::string creator = shaderDesc.Creator;

	// 入力レイアウトを取得する
	for (int k = 0; k < int(shaderDesc.InputParameters); k++)
	{
		// 入力情報を取得
		D3D11_SIGNATURE_PARAMETER_DESC paramDesc = {};
		reflector->GetInputParameterDesc(k, &paramDesc);

		ILinfo[k].SetSemanticName(paramDesc.SemanticName);
		ILinfo[k].SetSemanticIndex(int(paramDesc.SemanticIndex));
		ILinfo[k].SetInputSlot(0); // 通常は0

		// ComponentMask から DXGI_FORMAT を推定
		if (paramDesc.Mask == 1) {
			ILinfo[k].SetFormat(int(DXGI_FORMAT_R32_FLOAT));
		}
		else if (paramDesc.Mask <= 3) {
			ILinfo[k].SetFormat(int(DXGI_FORMAT_R32G32_FLOAT));
		}
		else if (paramDesc.Mask <= 7) {
			ILinfo[k].SetFormat(int(DXGI_FORMAT_R32G32B32_FLOAT));
		}
		else if (paramDesc.Mask <= 15) {
			ILinfo[k].SetFormat(int(DXGI_FORMAT_R32G32B32A32_FLOAT));
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
			CBInfo[i].SetName(bufferName);
			CBInfo[i].SetRegisterNumber(registerNumber);
			CBInfo[i].SetSize(bufferDesc.Size);
		}
		else {
			ErrorLog::OutputToConsole("バインド番号が見つかりませんでした");
			return false;
		}
	}

	return true;
}


// ================================================
// リファレンスした情報を出力する関数
// ================================================
bool ShaderInfoOutput(const char* kShaderInfoPath, std::vector<ShaderInfo>& shaderInfo)
{
	// フォルダがない場合作成
	if (!std::filesystem::exists(std::filesystem::path(kShaderInfoPath).parent_path())) {
		if (!std::filesystem::create_directories(std::filesystem::path(kShaderInfoPath).parent_path())) {
			ErrorLog::OutputToConsole("使用したシェイダーログ : ログフォルダの作成に失敗しました");
			return false;
		}
	}

	// セーブ情報を書き出す
	std::string data;
	for (int i = 0; i < (int)shaderInfo.size(); i++)
	{
		data += SaveUtils::FormatAnonymousBlock(shaderInfo[i].Serialize(2), 1);
		data += "\n";
	}
	data = SaveUtils::FormatBlock(kShaderStart, (int)shaderInfo.size(), data, 0);

	// ファイルを開ける
	std::ofstream ofs(kShaderInfoPath, std::ios::binary | std::ios::out);
	if (!ofs.is_open()) {
		ErrorLog::OutputToConsole("使用したシェイダーログ : ファイルのオープンに失敗しました");
		return false;
	}

	ofs << data;

	// ファイルを閉じる
	ofs.close();

	return true;
}

#endif