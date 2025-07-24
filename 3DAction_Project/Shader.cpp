#include "Shader.h"
#include <iostream>
#include <Windows.h>
#include <wrl/client.h>

#pragma comment(lib, "d3dcompiler.lib") // HLSLをコンパイルするためAPIを使用するためのライブラリー

// プロトタイプ宣言
namespace {
	std::string WStringToString(const std::wstring& wstr); // ワイド文字をマルチバイト文字にして返すヘルパー関数
}

// ======================================
// シェーダーをコンパイルする関数
// ======================================
bool CompileShader(const std::wstring fileName, const std::string entryPoint, const std::string shaderModel, ID3DBlob** ppBlobOut)
{
	HRESULT hr = S_OK; // 成功か失敗を返す

	// コンパイルフラグ
	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS; // コンパイル時に厳しくチェックするフラグ

#if defined(DEBUG) || defined(_DEBUG)
	dwShaderFlags |= D3DCOMPILE_DEBUG; // コンパイルしたシェーダーにデバッグ情報を付けるフラグ
#endif

#if !defined(DEBUG) && !defined(_DEBUG)
	dwShaderFlags |= D3DCOMPILE_OPTIMIZATION_LEVEL3; // 最高レベルの最適化を行うフラグ
#endif

	Microsoft::WRL::ComPtr<ID3DBlob> pErrorBlob = nullptr; // エラーを取得する

	hr = D3DCompileFromFile(
		fileName.c_str(),                  // シェーダーのファイル名
		nullptr,                           // GPUで使用するマクロ定義（ない場合nullptr）
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // HLSLで他のHLSLを読み込むフラグ
		entryPoint.c_str(),                // シェーダーないで最初に実行される関数の名前
		shaderModel.c_str(),               // シェーダーの種類とバージョン
		dwShaderFlags,                     // コンパイルのフラグ
		0,                                 // 今は何もないフラグ
		ppBlobOut,                         // コンパイルしたシェーダーを取得する
		pErrorBlob.GetAddressOf()          // エラーメッセージを取得する
	);

	if (FAILED(hr)) { // エラー取得時
		std::string messegeError = WStringToString(fileName) + "のコンパイルに失敗";
		MessageBoxA(nullptr, messegeError.c_str(), "エラー", MB_OKCANCEL | MB_ICONERROR); // メッセージボックス

#if defined(DEBUG) || defined(_DEBUG)
		if (pErrorBlob != nullptr) {  // デバッグ時のみ詳細なエラー出力
			std::cout << static_cast<const char*>(pErrorBlob->GetBufferPointer()) << std::endl;
		}
#endif
			return false;
	}

	// 解放処理
	if (pErrorBlob){
		pErrorBlob.Reset(); // 一応解放処理
	}

	return true;
}



namespace {

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