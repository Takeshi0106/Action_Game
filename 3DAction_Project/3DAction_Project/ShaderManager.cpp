#include "ShaderManager.h"

#include "ShaderData.h"  // シェイダークラス
#include <wrl/client.h>  // スマートポインター
#include <fstream>       //　ファイルの書出し 
#include <d3dcompiler.h> // シェーダーをコンパイルするヘッダー

#pragma comment(lib, "d3dcompiler.lib") // シェーダーをコンパイルするためAPIを使用するためのライブラリー


#include <Windows.h>     // エラーとエラーの種類を受け取るためにある　後で書き換える
#if defined(DEBUG) || defined(_DEBUG)
#include <iostream> // ウィンドウに書き出すよう
#endif

// ================================================
// 静的変数
// ================================================
std::unordered_map<std::string, VertexShaderData> ShaderManager::m_Vertexs;
std::unordered_map<std::string, PixelShaderData> ShaderManager::m_Pixels;
std::unordered_map<std::string, ComputeShaderData> ShaderManager::m_Computes;

// =================================================
// シェーダーを取得する関数
// =================================================



// =================================================
// シェーダーをコンパイルする関数
// =================================================
bool ShaderManager::OutputCompileShader(const ShaderInfo info)
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
		info.shaderTypeModel.c_str(),           // シェーダーの種類とバージョン
		dwShaderFlags,                          // コンパイルのフラグ
		0,                                      // 今は何もないフラグ
		compileBlob.GetAddressOf(),             // コンパイルしたシェーダーを取得する
		errorBlob.GetAddressOf()                // エラーメッセージを取得する
	);

	if (FAILED(hr)) { // エラー取得時
		std::string messegeError = info.name.string() + "のコンパイルに失敗";
		MessageBoxA(nullptr, messegeError.c_str(), "エラー", MB_OK | MB_ICONERROR); // メッセージボックス

#if defined(DEBUG) || defined(_DEBUG)
		if (errorBlob != nullptr) {  // デバッグ時のみ詳細なエラー出力
			std::cout << static_cast<const char*>(errorBlob->GetBufferPointer()) << std::endl;
		}
#endif
		return false;
	}

	std::string filename = info.name.stem().string();                            // 拡張子なしの名前を取得
	std::filesystem::path outputPath = kFilePath / (filename + ".cso");          // 出力パスを作成

	std::filesystem::create_directories(outputPath.parent_path()); // パスからフォルダを確認 なければを作成してくれる

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


// 頂点シェーダーを探す関数
VertexShaderData* ShaderManager::GetFindVertexShader(const std::string& name)
{
	auto it = m_Vertexs.find(name);
	if (it != m_Vertexs.end()) {
		return &(it->second);
	}
	return nullptr; // 見つからなかった
}
// ピクセルシェーダを探す関数
PixelShaderData* ShaderManager::GetFindPixelShader(const std::string& name)
{
	auto it = m_Pixels.find(name);
	if (it != m_Pixels.end()) {
		return &(it->second);
	}
	return nullptr; // 見つからなかった
}
// コンピュートシェーダを探す関数
ComputeShaderData* ShaderManager::GetFindComputeShader(const std::string& name)
{
	auto it = m_Computes.find(name);
	if (it != m_Computes.end()) {
		return &(it->second); 
	}
	return nullptr; // 見つからなかった
}