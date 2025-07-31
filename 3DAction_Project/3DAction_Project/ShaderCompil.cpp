// 必須ヘッダー
#include "ShaderCompil.h"

// シェイダーコンパイル用ヘッダー
#include <d3dcompiler.h>                // シェーダーをコンパイルするためのヘッダー
#pragma comment(lib, "d3dcompiler.lib") // シェーダーをコンパイルするためのライブラリー

// DirectX用スマートポインター
#include <wrl/client.h>  // DirectX用のスマートポインター

// ファイル出力、読込み用ヘッダー
#include <fstream>                      // 外部ファイルとして書出し・読み出し

// デバッグ情報ややエラー出力用
#include "ReportMessage.h"


// =================================================
// シェーダーをコンパイルする関数
// =================================================
bool OutputCompileShader(const std::filesystem::path kFilePath, const std::filesystem::path path, const std::string entryPoint, const std::string shaderTypeModel, ID3DBlob** blob)
{
	// エラーを取得する
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;
	// コンパイルしたバイナリーデータを入れる
	Microsoft::WRL::ComPtr<ID3DBlob> compileBlob = nullptr;

	// コンパイルフラグ
	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS; // コンパイル時に厳しくチェックするフラグ
#if defined(DEBUG) || defined(_DEBUG)
	dwShaderFlags |= D3DCOMPILE_DEBUG; // コンパイルしたシェーダーにデバッグ情報を付けるフラグ
#endif

#if !defined(DEBUG) && !defined(_DEBUG)
	dwShaderFlags |= D3DCOMPILE_OPTIMIZATION_LEVEL3; // 最高レベルの最適化を行うフラグ
#endif

	// コンパイル
	HRESULT hr = D3DCompileFromFile(
		path.wstring().c_str(),                 // シェーダーのパス
		nullptr,                                // GPUで使用するマクロ定義（ない場合nullptr）
		D3D_COMPILE_STANDARD_FILE_INCLUDE,      // HLSLで他のHLSLを読み込むフラグ
		entryPoint.c_str(),                     // シェーダーないで最初に実行される関数の名前
		shaderTypeModel.c_str(),                // シェーダーの種類とバージョン
		dwShaderFlags,                          // コンパイルのフラグ
		0,                                      // 今は何もないフラグ
		compileBlob.GetAddressOf(),             // コンパイルしたシェーダーを取得する
		errorBlob.GetAddressOf()                // エラーメッセージを取得する
	);
	if (!ErrorLog::IsSuccessHRESULTWithMessageBox(hr, std::string(path.string() + "のコンパイルに失敗").c_str())) {
		ErrorLog::Log(static_cast<const char*>(errorBlob->GetBufferPointer()));
		return false;
	}

	// 拡張子なしの名前を取得
	std::string filename = path.stem().string();
	// 出力パスを作成
	std::filesystem::path outputPath = kFilePath / (filename + kCompileExtension);

	// フォルダがないときフォルダを作成
	if (!std::filesystem::exists(outputPath.parent_path())) {
		if (!std::filesystem::create_directories(outputPath.parent_path())) {
			ErrorLog::Log("コンパイル : ログフォルダの作成に失敗しました");
			return false;
		}
	}

	// 書き出し処理
	std::ofstream ofs(outputPath, std::ios::binary | std::ios::out);
	if (!ofs) {
		MessageBoxA(nullptr, "CSOファイルの書き込みに失敗しました。", "エラー", MB_OK | MB_ICONERROR);
		return false;
	}

	// バイナリデータを書き出す
	ofs.write(static_cast<const char*>(compileBlob->GetBufferPointer()), compileBlob->GetBufferSize());
	// すぐに書き込んでもらう
	ofs.flush();
	// ファイルを閉じる
	ofs.close();

	// 所有権を渡す
	*blob = compileBlob.Detach();

	// 一応明示的に解放　
	errorBlob.Reset();
	compileBlob.Reset();

	return true;
}


// ============================================================================
// コンパイルして外部に書き出したファイルをBlobに取り込む関数
// ============================================================================
bool LoadCompiledShader(const std::filesystem::path& csoPath, ID3DBlob** blob)
{
	// ファイルを開ける
	std::ifstream ifs(csoPath, std::ios::binary | std::ios::ate); // 読み取り専用
	if (!ifs) {
		ErrorLog::Log(std::string(csoPath.string() + "　：　開けませんでした").c_str());
		return false;
	}

	// ファイルサイズを取得する
	std::streamsize size = ifs.tellg();
	if (size <= 0) {
		ErrorLog::Log(std::string(csoPath.string() + " : ファイルサイズ取得に失敗しました").c_str());
		return false;
	}

	// ファイルを先頭に戻す
	ifs.seekg(0, std::ios::beg);

	// ID3DBlobを作成する
	HRESULT hr = D3DCreateBlob(static_cast<SIZE_T>(size), blob);
	if (!ErrorLog::IsSuccessHRESULTWithOutputToConsole(hr, "バイナリーデータの作製に失敗しました")) {
		ifs.close(); // ファイルを閉じる
		return false;
	}
	// 中にバイナリーデータを書き込む
	ifs.read(reinterpret_cast<char*>((*blob)->GetBufferPointer()), size);
	if (!ifs) {
		ErrorLog::Log(std::string(csoPath.string() + " : バイナリーデータの読み込みに失敗しました").c_str());
		ifs.close();
		return false;
	}

	ifs.close(); // ファイルを閉じる

	return true;
}