// 必須ヘッダー
#include "ShaderManager.h" // 自分のヘッダー
#include "ShaderData.h"    // シェイダーデータクラス

// シェイダーコンパイル用ヘッダー
#include <d3dcompiler.h>                // シェーダーをコンパイルするためのヘッダー
#pragma comment(lib, "d3dcompiler.lib") // シェーダーをコンパイルするためのライブラリー
#include <fstream>                      // 外部ファイルとして書出し
#include <Windows.h>                    // デバイス受け取り

// DirectX用
#include <wrl/client.h>  // DirectX用のスマートポインター

// デバッグ情報ややエラー出力用
#include "ReportMessage.h"

// ファイルパス用ヘッダー
#include <filesystem>  // ファイルパスなどを楽に扱える　C++17以降

#if defined(DEBUG) || defined(_DEBUG)
#include <vector>  // デバッグ時にシェイダーの名前を保存しておく用
#endif


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
bool LoadCompiledShaderBlob(const std::filesystem::path& csoPath, ID3DBlob** blob);    // パスから.csoを読み込んでくる関数


#if defined(DEBUG) || defined(_DEBUG)
bool IsShaderUpdateCheck(const std::filesystem::path& shaderPath, const std::filesystem::path& binaryPath); // .hlslが更新されているかを確認する
#endif


// =================================================
// 初期化
// =================================================
bool ShaderManager::Init(ID3D11Device* device)
{
	bool IsSuccess = true;

#if defined(DEBUG) || defined(_DEBUG)
	// デバッグ用初期化
	IsSuccess = DebugInit(device); // デバッグ時のみ同じ階層にある 全ての.hlslをコンパイルする
#else

#endif

	return IsSuccess;
}


// =================================================
// 後処理
// =================================================
void ShaderManager::Uninit()
{
	m_Vertexs.clear();  // 中身を削除 unique_ptrのためDeleteされる
	m_Pixels.clear();   // 中身を削除 unique_ptrのためDeleteされる
	m_Computes.clear(); // 中身を削除 unique_ptrのためDeleteされる
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
bool ShaderManager::DebugInit(ID3D11Device* device) // 同じ階層にある.hlslを全てコンパイルして書き出す 将来的には更新されているかを確認してコンパイルするかしないかを判定したい
{
	std::filesystem::path currentFilePath = __FILE__;                       // このソースコードのパスを取得
	std::filesystem::path currentDirectory = currentFilePath.parent_path(); // パスから自分の階層だけを抜き取る
	bool IsSuccess = true; // 成否判定
	std::vector<std::string> shaderNames; // 名前取得用

	// .hlslファイルを探す
	for (const auto& entry : std::filesystem::directory_iterator(currentDirectory)) // 階層内のファイルを全て取得しています
	{
		if (!entry.is_regular_file() || entry.path().extension() != ".hlsl") { continue; } // ファイルでなかったり、拡張子が違ったりすれば次のループへ

		std::filesystem::path hlslPath = entry.path();                                                            // .hlslのフルパスをを取得
		std::filesystem::path filename = hlslPath.filename();                                                     // .hlslのファイル名を取得
		std::filesystem::path csoPath = std::filesystem::path(kFilePath) / (filename.stem().wstring() + L".cso"); // .hlslの拡張子なしファイル名に.csoを付けてコンパイル名を取得

		Microsoft::WRL::ComPtr<ID3DBlob> blob; // バイナリーデータ入れる

		if (IsShaderUpdateCheck(hlslPath, csoPath)) { // 更新日時を調べる

			// ファイルの最初の名前でシェーダー判定
			if (filename.string().rfind("VS_", 0) == 0) {
				if (!OutputCompileShader(kFilePath, filename, "main", "vs_5_0", blob.GetAddressOf())) // コンパイルして書き出す
				{
					ErrorLog::Log("頂点シェーダーのコンパイル失敗");
					IsSuccess = false;
				}
			}
			else if (filename.string().rfind("PS_", 0) == 0) {
				if(!OutputCompileShader(kFilePath, filename, "main", "ps_5_0", blob.GetAddressOf())) // コンパイルして書き出す
				{
					ErrorLog::Log("ピクセルシェーダーのコンパイル失敗");
					IsSuccess = false;
				}
			}
			else if (filename.string().rfind("CS_", 0) == 0) {
				if(!OutputCompileShader(kFilePath, filename, "main", "cs_5_0", blob.GetAddressOf())) // コンパイルして書き出す
				{
					ErrorLog::Log("コンピュートシェーダのコンパイル失敗");
					IsSuccess = false;
				}
			}
			else {
				ErrorLog::Log(std::string(filename.string() + " : 先頭にシェーダーの種類が記載されていません").c_str()); // ログ出力
				IsSuccess = false;
			}
			if (!IsSuccess) {
				ErrorLog::MessageBoxOutput("シェイダーの初期化に失敗しました");
				return false;
			}
		}
		else {			// .cso を読み込む
			if (!LoadCompiledShaderBlob(csoPath, blob.GetAddressOf())) {
				ErrorLog::MessageBoxOutput((csoPath.string() + " : CSOの読み込みに失敗しました").c_str());
				return false;
			}
		}

		// バイナリーデータをいれて、シェーダーを作成　配列に代入
		if (filename.string().rfind("VS_", 0) == 0) {
			auto vertex = std::make_unique<VertexShaderData>(filename.stem().string(), "main", "vs_5_0");  // 動的確保
			IsSuccess = vertex->Init(device, blob.Get()->GetBufferPointer(), blob.Get()->GetBufferSize()); // 初期化実行
			m_Vertexs[filename.stem().string()] = std::move(vertex);                                       // メンバー配列に代入

			shaderNames.push_back(filename.stem().string());                                               // 名前を保存しておく
		}
		else if (filename.string().rfind("PS_", 0) == 0) {
			auto pixel = std::make_unique<PixelShaderData>(filename.stem().string(), "main", "ps_5_0");      // 動的確保
			IsSuccess = pixel->Init(device, blob.Get()->GetBufferPointer(), blob.Get()->GetBufferSize());    // 初期化実行
			m_Pixels[filename.stem().string()] = std::move(pixel);                                           // メンバー配列に代入

			shaderNames.push_back(filename.stem().string());                                                 // 名前を保存しておく
		}
		else if (filename.string().rfind("CS_", 0) == 0) {
			auto compute = std::make_unique< ComputeShaderData>(filename.stem().string(), "main", "cs_5_0"); // 動的確保
			IsSuccess = compute->Init(device, blob.Get()->GetBufferPointer(), blob.Get()->GetBufferSize());  // 初期化実行
			m_Computes[filename.stem().string()] = std::move(compute);                                       // メンバー配列に代入

			shaderNames.push_back(filename.stem().string());                                                 // 名前を保存しておく
		}
		else {
			ErrorLog::Log(std::string(filename.string() + " : 先頭にシェーダーの種類が記載されていません").c_str()); // ログ出力
			IsSuccess = false;
		}
		if (!IsSuccess) {
			ErrorLog::MessageBoxOutput("シェイダーの初期化に失敗しました");
			return false;
		}

		blob.Reset(); // 一応、解放処理
	}

	// 使用したシェイダーの名前をテキストファイルに書き出す処理
	// フォルダがない場合作成
	if (!std::filesystem::exists(std::filesystem::path(kAssetLogPath).parent_path())) { // ファイルがない場合作成する
		if (!std::filesystem::create_directories(std::filesystem::path(kAssetLogPath).parent_path())) {
			ErrorLog::Log("使用したシェイダーログ : ログフォルダの作成に失敗しました");
			return false;
		}
	}

	std::ofstream ofs(kAssetLogPath, std::ios::binary | std::ios::out); // ファイルを開ける

	for (const std::string& name : shaderNames) {
		ofs << name << "\n";                                              // ファイルに書き込み
	}
	ofs.close();                                                          // ファイルを閉じる

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

	if (!ErrorLog::IsSuccessHRESULTWithMessageBox(hr, std::string(name.string() + "のコンパイルに失敗").c_str())) {
		ErrorLog::Log(static_cast<const char*>(errorBlob->GetBufferPointer()));
		return false;
	}

	std::string filename = name.stem().string();                                 // 拡張子なしの名前を取得
	std::filesystem::path outputPath = kFilePath / (filename + ".cso");          // 出力パスを作成

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

	ofs.write(static_cast<const char*>(compileBlob->GetBufferPointer()), compileBlob->GetBufferSize());   // バイナリデータを書き出す
	ofs.close();                                                                                          // ファイルを閉じる

	*blob = compileBlob.Detach();

	// 一応明示的に解放　
	errorBlob.Reset();
	compileBlob.Reset();

	return true;
}


// コンパイルして外部に書き出したファイルをBlobに取り込む関数
bool LoadCompiledShaderBlob(const std::filesystem::path& csoPath, ID3DBlob** blob)
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
	ifs.close(); // ファイルを閉じる

	return true;
}



#if defined(DEBUG) || defined(_DEBUG)
bool IsShaderUpdateCheck(const std::filesystem::path& shaderPath, const std::filesystem::path& binaryPath)
{
	try {
		// .csoファイルが存在しなければ必ず更新ありと判定
		if (!std::filesystem::exists(binaryPath)) {
			return true;
		}

		// 両ファイルの更新日時取得
		auto shaTime = std::filesystem::last_write_time(shaderPath);
		auto binTime = std::filesystem::last_write_time(binaryPath);

		// .hlslのほうが新しければ更新
		if (shaTime > binTime) {
			ErrorLog::Log(std::string(shaderPath.stem().string() + " が新しく更新されています").c_str());
			return true;
		}
	}
	catch (const std::filesystem::filesystem_error& e) {
		// ファイルアクセスに失敗した場合もコンパイル必須にする
		ErrorLog::Log(std::string(std::string("IsShaderUpdateCheck ファイルアクセスエラー: ") + e.what()).c_str());
		return true;
	}

	ErrorLog::Log(std::string(shaderPath.stem().string() + " 更新する必要がないためコンパイルファイルを取得します  相対パス : " + binaryPath.string()).c_str());
	return false;
}


#endif