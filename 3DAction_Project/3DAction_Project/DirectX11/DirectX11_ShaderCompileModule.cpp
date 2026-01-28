
// =====================================
// ヘッダー
// =====================================
// 必須ヘッダー
#include "DirectX11_ShaderCompileModule.h"
// シェイダーコンパイル用ヘッダー
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
// DirectX用スマートポインター
#include <wrl/client.h> 
// ファイル出力、読込み用ヘッダー
#include <fstream> 
// ファイルシステムヘッダー
#include <filesystem>
// 文字列
#include "../UTF8_String.h"
// デバッグ情報ややエラー出力用
#include "../ReportMessage.h"


// =====================================
// グローバル関数
// =====================================
// コンパイルするかチェッククラス
bool IsCompileCheck(
	const std::filesystem::path& _hlslPath, 
	const std::filesystem::path& _compilePath);

// シェーダーを判定してコンパイル依頼する関数
bool Chacke_ShaderCompaile(
	const std::filesystem::path _hlslPath, 
	const std::filesystem::path _compilePath, 
	Microsoft::WRL::ComPtr<ID3DBlob>& blob);

// シェーダーをコンパイルして出力する関数
bool OutputCompileShader(
	const std::filesystem::path _hlslPath,
	const std::filesystem::path _compilePath,
	const String& entryPoint,
	const String& shaderTypeModel,
	Microsoft::WRL::ComPtr<ID3DBlob>& blob);


// =====================================
// コンパイルチェック
// =====================================
#if defined(_DEBUG) || defined(DEBUG)
void DirectX11_ShaderCompileModule::ShaderCompil()
{
	// このソースコードのパスを取得
	std::filesystem::path currentFilePath = __FILE__;
	// パスから自分の階層だけを抜き取る
	std::filesystem::path currentDirectory = currentFilePath.parent_path();

	// .hlslファイル数を見積もる
	size_t shaderFileCount = std::count_if(
		std::filesystem::directory_iterator(currentDirectory),
		std::filesystem::directory_iterator{},
		[this](const auto& entry) {
			return entry.is_regular_file() && entry.path().extension() == (std::filesystem::path)kHlslExtension.GetU8String(); 
		});

	// インデックス
	uint16_t hlslCount = 0;

	// .hlslファイルを探す処理
	for (const auto& entry : std::filesystem::directory_iterator(currentDirectory))
	{
		// 階層内の全てのファイルをを所得して、ファイルでなかったり、拡張子が違ったりすれば次のループへ
		if (!entry.is_regular_file() || entry.path().extension() != (std::filesystem::path)khlslPath.GetU8String()) { continue; }

		// 念のためチェック
		if ((uint16_t)shaderFileCount < hlslCount) {
			ErrorLog::OutputToConsole(u8".hlslファイルの数が一致しません");
		}

		// .hlslのパスをを取得
		std::filesystem::path hlslPath = entry.path();
		// コンパイルパスを作成
		std::filesystem::path compilePath = 
			std::filesystem::path(kCompilPath.GetU8String()) / 
			(hlslPath.filename().stem().u8string() + 
			kCompilExtension.GetU8String());

		// 区切り文字を統一
		hlslPath = hlslPath.generic_string();
		compilePath = compilePath.generic_string();

		// シェーダーのコンパイルする必要があるかのチェック
		if (!IsCompileCheck(hlslPath, compilePath)) { 
			continue; 
		}

		// シェーダーコンパイル　解析(リファレンス)を行うため、Blobを用意
		Microsoft::WRL::ComPtr<ID3DBlob> shaderBlob = nullptr;

		// シェーダーの種類を判定してコンパイル依頼
		if (!Chacke_ShaderCompaile(hlslPath, compilePath, shaderBlob)) {
			ErrorLog::OutputToConsole(u8"シェーダーのコンパイルに失敗しました: " + hlslPath.u8string());
			continue;
		}

		// カウントを増やす
		hlslCount++;

		// 解放
		shaderBlob.Reset();
	}
}

#else
void DirectX11_ShaderCompileModule::ShaderCompil()
{
	// リリースビルドでは何もしない
}

#endif


// =====================================
// コンパイルするかのチェック関数
// =====================================
bool IsCompileCheck(
	const std::filesystem::path& _hlslPath,
	const std::filesystem::path& _compilePath)
{
	// シェーダーファイルの最終更新日時を取得
	auto shaderLastWriteTime = std::filesystem::last_write_time(_hlslPath);

	// バイナリファイルが存在しない場合は更新が必要
	if (!std::filesystem::exists(_compilePath)) {
		return true;
	}

	// バイナリファイルの最終更新日時を取得
	auto binaryLastWriteTime = std::filesystem::last_write_time(_hlslPath);

	// シェーダーファイルの方が新しい場合は更新が必要
	return shaderLastWriteTime > binaryLastWriteTime;
}


// =====================================
// シェーダーコンパイル判定関数
// =====================================
bool Chacke_ShaderCompaile(
	const std::filesystem::path _hlslPath,
	const std::filesystem::path _compilePath, 
	Microsoft::WRL::ComPtr<ID3DBlob>& blob)
{

	// ファイルの最初の名前でシェーダー判定
	if (_hlslPath.stem().u8string().rfind(u8"PS_", 0) == 0)
	{
		// コンパイルして書き出す
		if (!OutputCompileShader(_hlslPath, _compilePath, u8"main", u8"ps_5_0", blob)) {
			ErrorLog::OutputToConsole(u8"ピクセルシェーダー " + _hlslPath.u8string() + u8" のコンパイル失敗");
			return false;
		}
	}
	else if (_hlslPath.stem().u8string().rfind(u8"VS_", 0) == 0)
	{
		// コンパイルして書き出す
		if (!OutputCompileShader(_hlslPath, _compilePath, u8"main", u8"vs_5_0", blob)) {
			ErrorLog::OutputToConsole(u8"頂点シェーダー " + _hlslPath.u8string() + u8" のコンパイル失敗");
			return false;
		}
	}
	else if (_hlslPath.stem().string().rfind("CS_", 0) == 0)
	{
		// コンパイルして書き出す
		if (!OutputCompileShader(_hlslPath, _compilePath, u8"main", u8"cs_5_0", blob)) {
			ErrorLog::OutputToConsole(u8"コンピュートシェーダ " + _hlslPath.u8string() + u8" のコンパイル失敗");
			return false;
		}
	}
	else
	{
		ErrorLog::OutputToConsole(_hlslPath.u8string() + u8" : 先頭にシェーダーの種類が記載されていません");
		return  false;
	}

	return true;
}


// =====================================
// シェーダーをコンパイルして出力する関数
// =====================================
bool OutputCompileShader(
	const std::filesystem::path _hlslPath,
	const std::filesystem::path _compilepath,
	const String& entryPoint,
	const String& shaderTypeModel,
	Microsoft::WRL::ComPtr<ID3DBlob>& blob)
{
	// エラーを取得する
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;

	// コンパイルフラグ
	// コンパイル時に厳しくチェックするフラグ
	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

#if defined(DEBUG) || defined(_DEBUG)
	// デバッグ情報を付けるフラグ
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#else
	// 最適化フラグ
	dwShaderFlags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif

	// コンパイル
	HRESULT hr = D3DCompileFromFile(
		_hlslPath.wstring().c_str(),            // シェーダーのパス
		nullptr,                                // GPUで使用するマクロ定義（ない場合nullptr）
		D3D_COMPILE_STANDARD_FILE_INCLUDE,      // HLSLで他のHLSLを読み込むフラグ
		reinterpret_cast<const char*>(entryPoint.GetU8Char()),                     // シェーダーないで最初に実行される関数の名前
		reinterpret_cast<const char*>(shaderTypeModel.GetU8Char()),                // シェーダーの種類とバージョン
		dwShaderFlags,                          // コンパイルのフラグ
		0,                                      // 今は何もないフラグ
		blob.GetAddressOf(),                    // コンパイルしたシェーダーを取得する
		errorBlob.GetAddressOf()                // エラーメッセージを取得する
	);
	if (FAILED(hr)) {
		ErrorLog::OutputToConsole(_hlslPath.u8string() + u8"のコンパイルに失敗" + (static_cast<const char8_t*>(errorBlob->GetBufferPointer())));
		return false;
	}

	// 書き出す内容を作成
	std::string_view blobData(
		static_cast<const char*>(blob->GetBufferPointer()),
		blob->GetBufferSize()
	);

	// フォルダがない場合作成
	if (!std::filesystem::exists(_compilepath.parent_path())) 
	{
		if (!std::filesystem::create_directories(_compilepath.parent_path())) 
		{
			ErrorLog::OutputToConsole(u8"ファイルが作成できませんでした");
			return false;
		}
	}

	// バイナリモードで書き出し
	// ファイルを開く
	std::ofstream ofs(_compilepath, std::ios::binary | std::ios::out);
	if (!ofs)
	{
		ErrorLog::OutputToConsole(u8"ファイルを開けませんでした: " + _compilepath.u8string());
		return false;
	}

	// データを書き出す
	ofs.write(blobData.data(), blobData.size());
	if (!ofs)
	{
		ErrorLog::OutputToConsole(u8"ファイル書き込みに失敗しました: " + _compilepath.u8string());
		return false;
	}

	// ファイルを閉じる
	ofs.close();
	// 解放
	errorBlob.Reset();

	return true;
}
