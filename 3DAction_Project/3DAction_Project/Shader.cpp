#include "Shader.h"
#include <iostream>
#include <Windows.h>
#include <wrl/client.h>
#include <fstream>
#include <d3dcompiler.h>

#pragma comment(lib, "d3dcompiler.lib") // HLSLをコンパイルするためAPIを使用するためのライブラリー

// プロトタイプ宣言
namespace {
	std::string ShaderModelToString(const ShaderType type); // シェーダーモデルが5.0のみのため、シェーダーの種類を渡して文字列を返す関数
	std::string WStringToString(const std::wstring& wstr); // ワイド文字をマルチバイト文字にして返すヘルパー関数
}

// ======================================
// シェーダーをコンパイルする関数
// ======================================
bool OutputCompileShader(const ShaderInfo info, const std::string outputFilePath)
{
	HRESULT hr = S_OK;                                      // 成功か失敗を返す
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;   // エラーを取得する
	Microsoft::WRL::ComPtr<ID3DBlob> compileBlob = nullptr; // コンパイルしたバイナリーデータを入れる

	// コンパイルフラグ
	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS; // コンパイル時に厳しくチェックするフラグ
#if defined(DEBUG) || defined(_DEBUG)
	dwShaderFlags |= D3DCOMPILE_DEBUG; // コンパイルしたシェーダーにデバッグ情報を付けるフラグ
#endif

#if !defined(DEBUG) && !defined(_DEBUG)
	dwShaderFlags |= D3DCOMPILE_OPTIMIZATION_LEVEL3; // 最高レベルの最適化を行うフラグ
#endif

	hr = D3DCompileFromFile(
		info.name.wstring().c_str(),            // シェーダーのファイル名
		nullptr,                                // GPUで使用するマクロ定義（ない場合nullptr）
		D3D_COMPILE_STANDARD_FILE_INCLUDE,      // HLSLで他のHLSLを読み込むフラグ
		info.entryPoint.c_str(),                // シェーダーないで最初に実行される関数の名前
		ShaderModelToString(info.type).c_str(), // シェーダーの種類とバージョン
		dwShaderFlags,                          // コンパイルのフラグ
		0,                                      // 今は何もないフラグ
		compileBlob.GetAddressOf(),             // コンパイルしたシェーダーを取得する
		errorBlob.GetAddressOf()                // エラーメッセージを取得する
	);

	if (FAILED(hr)) { // エラー取得時
		std::string messegeError =info.name.string() + "のコンパイルに失敗";
		MessageBoxA(nullptr, messegeError.c_str(), "エラー", MB_OK | MB_ICONERROR); // メッセージボックス

#if defined(DEBUG) || defined(_DEBUG)
		if (errorBlob != nullptr) {  // デバッグ時のみ詳細なエラー出力
			std::cout << static_cast<const char*>(errorBlob->GetBufferPointer()) << std::endl;
		}
#endif
		return false;
	}

	// 出力パスが設定されたいなかったらエラー
	if (outputFilePath.empty()) {
		MessageBoxA(nullptr, "コンパイルファイルの出力先パスが設定されていません", "エラー", MB_OK | MB_ICONERROR);
		return false;
	}

	std::string filename = info.name.stem().string();                      // 拡張子なしの名前を取得
	std::string outputPath = outputFilePath + filename + ".cso";           // 出力パスを作成

	// 書き出し処理
	std::ofstream ofs(outputPath, std::ios::binary | std::ios::out);
	if (!ofs) {
		MessageBoxA(nullptr, "CSOファイルの書き込みに失敗しました。", "エラー", MB_OK | MB_ICONERROR);
		return false;
	}
	ofs.write(static_cast<const char*>(compileBlob->GetBufferPointer()), compileBlob->GetBufferSize());   // バイナリデータを書き出す
	ofs.close();                                                                                          // ファイルを閉じる

	// 一応解放
	errorBlob.Reset();
	compileBlob.Reset();

	return true;
}



namespace {

	// シェーダーの種類を渡して、コンパイルに必要な情報を返す関数
	std::string ShaderModelToString(const ShaderType type) {

		std::string sharderInf;

		switch (type) {
		case ShaderType::VERTEX_5_0:
			sharderInf = "vs_5_0";
			break;

		case ShaderType::PIXEL_5_0:
			sharderInf = "ps_5_0";
			break;

		case ShaderType::GEOMETRY_5_0:
			sharderInf = "gs_5_0";
			break;

		case ShaderType::HULL_5_0:
			sharderInf = "hs_5_0";
			break;

		case ShaderType::DOMAIN_5_0:  
			sharderInf = "ds_5_0";
			break;

		case ShaderType::COMPUTE_5_0:
			sharderInf = "cs_5_0";
			break;
		}

		return sharderInf;
	}


	// ワイド文字をマルチバイトにして返すヘルパー関数
	std::string WStringToString(const std::wstring& wstr)
	{
		if (wstr.empty()) return std::string(); // 空なら戻る

		int size = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr); // 引き数のワイド文字をマルチバイトに変換するためのサイズを取得
		std::string result(size, '\0');                                                            // 文字列を作成して初期化
		WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, &result[0], size, nullptr, nullptr);      // 文字列を変換して代入する関数
		result.pop_back();                                                                         // 変換時に代入した-1(終焉文字)を取り出す

		return result;
	}


}