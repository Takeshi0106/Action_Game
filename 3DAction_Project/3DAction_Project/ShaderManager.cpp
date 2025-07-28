// 必須ヘッダー
#include "ShaderManager.h" // 自分のヘッダー
#include "ShaderData.h"    // シェイダーデータクラス

// シェイダーコンパイル用ヘッダー
#include <d3dcompiler.h>                // シェーダーをコンパイルするためのヘッダー
#pragma comment(lib, "d3dcompiler.lib") // シェーダーをコンパイルするためのライブラリー
#include <fstream>                      // 外部ファイルとして書出し
#include <Windows.h>                    // デバイス受け取り

// DirectX用
#include <wrl/client.h>                 // DirectX用のスマートポインター

// デバッグ情報ややエラー出力用
#include "ReportMessage.h"


// ================================================
// 静的変数
// ================================================
std::unordered_map<std::string, std::unique_ptr<VertexShaderData>>  ShaderManager::m_Vertexs;
std::unordered_map<std::string, std::unique_ptr<PixelShaderData>>   ShaderManager::m_Pixels;
std::unordered_map<std::string, std::unique_ptr<ComputeShaderData>> ShaderManager::m_Computes;


// =================================================
// プロトタイプ宣言
// =================================================
bool OutputCompileShader(const std::filesystem::path kFilePath,const std::filesystem::path name, 
	const std::string entryPoint, const std::string shaderTypeModel, ID3DBlob** blob); // シェーダーをコンパイルして外部ファイルに書き出し引き数のblobにバイナリデータを入れる


// =================================================
// 初期化
// =================================================
void ShaderManager::Init(ID3D11Device* device)
{

#if defined(DEBUG) || defined(_DEBUG)
	// デバッグ用初期化


#endif
}


// 頂点シェーダーを探す関数
VertexShaderData* ShaderManager::GetFindVertexShader(const std::string& name)
{
	auto it = m_Vertexs.find(name);
	if (it != m_Vertexs.end()) {
		return it->second.get();
	}
	return nullptr; // 見つからなかった
}
// ピクセルシェーダを探す関数
PixelShaderData* ShaderManager::GetFindPixelShader(const std::string& name)
{
	auto it = m_Pixels.find(name);
	if (it != m_Pixels.end()) {
		return it->second.get();
	}
	return nullptr; // 見つからなかった
}
// コンピュートシェーダを探す関数
ComputeShaderData* ShaderManager::GetFindComputeShader(const std::string& name)
{
	auto it = m_Computes.find(name);
	if (it != m_Computes.end()) {
		return it->second.get(); 
	}
	return nullptr; // 見つからなかった
}


#if defined(DEBUG) || defined(_DEBUG)

// デバッグ時のみ有効にする関数
bool ShaderManager::CompileAllHLSLFilesInDirectory(ID3D11Device* device) // 同じ階層にある.hlslを全てコンパイルして書き出す 将来的には更新されているかを確認してコンパイルするかしないかを判定したい
{
	std::filesystem::path currentFilePath = __FILE__;                       // このソースコードのパスを取得
	std::filesystem::path currentDirectory = currentFilePath.parent_path(); // パスから自分の階層だけを抜き取る
	bool IsSuccess = true; // 成否判定

	// .hlslファイルを探す
	for (const auto& entry : std::filesystem::directory_iterator(currentDirectory)) // 階層内のファイルを全て取得しています
	{
		if (!entry.is_regular_file() || entry.path().extension() != ".hlsl") { continue; } // ファイルでなかったり、拡張子が違ったりすれば次のループへ

		Microsoft::WRL::ComPtr<ID3DBlob> blob;
		std::filesystem::path filename = entry.path().filename();

		if (filename.string().rfind("VS_", 0) == 0) {
			OutputCompileShader(kFilePath, filename.stem(), "main", "vs_5_0", blob.GetAddressOf());       // コンパイルして書き出す
			auto vertex = std::make_unique<VertexShaderData>(filename.stem().string(), "main", "vs_5_0"); // 動的確保
			IsSuccess = vertex->Init(device, blob.Get());                                                 // 初期化実行
			m_Vertexs[filename.stem().string()] = std::move(vertex);                                      // メンバー配列に代入
		}
		else if (filename.string().rfind("PS_", 0) == 0) {
			OutputCompileShader(kFilePath, filename.stem().string(), "main", "ps_5_0", blob.GetAddressOf()); // コンパイルして書き出す
			auto pixel = std::make_unique<PixelShaderData>(filename.stem().string(), "main", "ps_5_0");      // 動的確保
			IsSuccess = pixel->Init(device, blob.Get());                                                     // 初期化実行
			m_Pixels[filename.stem().string()] = std::move(pixel);                                           // メンバー配列に代入
		}
		else if (filename.string().rfind("CS_", 0) == 0) {
			OutputCompileShader(kFilePath, filename.stem().string(), "main", "cs_5_0", blob.GetAddressOf()); // コンパイルして書き出す
			auto compute = std::make_unique< ComputeShaderData>(filename.stem().string(), "main", "cs_5_0"); // 動的確保
			IsSuccess = compute->Init(device, blob.Get());                                                   // 初期化実行
			m_Computes[filename.stem().string()] = std::move(compute);                                       // メンバー配列に代入
		}

		if (!IsSuccess){
			ErrorLog::MessageBoxOutput("シェイダーの初期化に失敗しました");
			return false;
		}

	}

	return true;

}


#endif


// =================================================
// シェーダーをコンパイルする関数
// =================================================
bool OutputCompileShader(const std::filesystem::path kFilePath, const std::filesystem::path name, const std::string entryPoint, const std::string shaderTypeModel, ID3DBlob** blob)
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
		name.wstring().c_str(),                 // シェーダーのファイル名
		nullptr,                                // GPUで使用するマクロ定義（ない場合nullptr）
		D3D_COMPILE_STANDARD_FILE_INCLUDE,      // HLSLで他のHLSLを読み込むフラグ
		entryPoint.c_str(),                     // シェーダーないで最初に実行される関数の名前
		shaderTypeModel.c_str(),                // シェーダーの種類とバージョン
		dwShaderFlags,                          // コンパイルのフラグ
		0,                                      // 今は何もないフラグ
		compileBlob.GetAddressOf(),             // コンパイルしたシェーダーを取得する
		errorBlob.GetAddressOf()                // エラーメッセージを取得する
	);

	if (ErrorLog::IsSuccessHRESULTWithMessageBox(hr, std::string(name.string() + "のコンパイルに失敗").c_str())) {
		ErrorLog::Log(static_cast<const char*>(errorBlob->GetBufferPointer()));
		return false;
	}

	std::string filename = name.stem().string();                                 // 拡張子なしの名前を取得
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

	*blob = compileBlob.Detach();

	// 一応明示的に解放　
	errorBlob.Reset();
	compileBlob.Reset();

	return true;
}