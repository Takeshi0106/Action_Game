// 必須ヘッダー
#include "ShaderManager.h" // 自分のヘッダー
#include "ShaderData.h"    // シェイダーデータクラス
// シェイダーコンパイル用ヘッダー
#include <d3dcompiler.h>                // シェーダーをコンパイルするためのヘッダー
#pragma comment(lib, "d3dcompiler.lib") // シェーダーをコンパイルするためのライブラリー
#include <Windows.h>                    // デバイス受け取り
// ファイル出力、読込み用ヘッダー
#include <filesystem>                   // ファイルパスなどを楽に扱える　C++17以降
#include <fstream>                      // 外部ファイルとして書出し・読み出し
#include <sstream>                      // 外部ファイル読み出し用
// DirectX用スマートポインター
#include <wrl/client.h>  // DirectX用のスマートポインター
// デバッグ情報ややエラー出力用
#include "ReportMessage.h"


// ================================================
// 静的変数
// ================================================
std::unordered_map<std::string, std::unique_ptr<VertexShaderData>>  ShaderManager::m_Vertexs;
std::unordered_map<std::string, std::unique_ptr<PixelShaderData>>   ShaderManager::m_Pixels;
std::unordered_map<std::string, std::unique_ptr<ComputeShaderData>> ShaderManager::m_Computes;


// ================================================
// グローバル変数
// ================================================
namespace {
	const std::string kShaderExtension = ".hlsl";
	const std::string kCompileExtension = ".cso";
}


// =================================================
// メンバー配列
// =================================================
// 定数バッファ情報の構造体
struct ConstantBufferInfo {
	std::string name = "";
	int bindSlot = 0;
	size_t size = 0;
};


// =================================================
// プロトタイプ宣言
// =================================================
bool OutputCompileShader(const std::filesystem::path kFilePath, const std::filesystem::path name, // シェーダーをコンパイルして外部ファイルに書き出し引き数のblobにバイナリデータを入れる
	const std::string entryPoint, const std::string shaderTypeModel, ID3DBlob** blob);
bool LoadCompiledShaderBlob(const std::filesystem::path& csoPath, ID3DBlob** blob);               // パスから.csoを読み込んでくる関数
bool JudgeCompileShader(const std::filesystem::path kFilePath,                                    // コンパイルするシェイダーの種類を判定してコンパイル関数に渡す
	const std::filesystem::path filename, Microsoft::WRL::ComPtr<ID3DBlob>& blob);
bool Reflect(void* blob, size_t blobsize, std::vector<ConstantBufferInfo>& outbuffer);              // リフレクション

#if defined(DEBUG) || defined(_DEBUG)
bool IsShaderUpdateCheck(const std::filesystem::path& shaderPath, const std::filesystem::path& binaryPath); // .hlslが更新されているかを確認する
#endif


// =================================================
// 初期化
// =================================================
bool ShaderManager::Init(ID3D11Device* device)
{
#if defined(DEBUG) || defined(_DEBUG)
	// デバッグ用初期化
	if (!DebugInit(device)) { // デバッグ時のみ同じ階層にある 全ての.hlslをコンパイルする
		ErrorLog::MessageBoxOutput("Debug : シェーダーマネージャーの初期化失敗");
		return false;
	}
#else
	if (!ReleaseInit(device)) {
		ErrorLog::Log("シェーダ―マネージャー初期化失敗");
		return false;
	}
#endif

	return true;
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


// ==================================================
//	リリース初期化関数
// ==================================================
// 外部ファイル シェーダー表を取得して、コンパイルされているかを確認
bool ShaderManager::ReleaseInit(ID3D11Device* device)
{
	std::ostringstream  shaderTable;  // 中身を入れる変数

	std::ifstream ifs(kShader_ConstantInfoPath, std::ios::binary); // ファイルを開ける
	if (!ifs.is_open()) {
		ErrorLog::MessageBoxOutput("シェイダー表を開けませんでした");
		return false;
	}

	shaderTable << ifs.rdbuf();       // 外部ファイルを全て代入する
	ifs.close();                      // ファイルを閉じる

	std::istringstream iss(shaderTable.str()); // 読み取り用ストリーム
	std::string line;                          // １行を入れる文字列

	while (std::getline(iss, line)) { // １行ずつ取り出す
		std::filesystem::path hlslPath = std::filesystem::path(kHlslFailePath) / (line + kShaderExtension); // シェーダーパスを作成
		hlslPath = hlslPath.generic_string(); // 区切り文字を / で統一する　

		std::filesystem::path compailPath = std::filesystem::path(kFilePath) / (line + kCompileExtension); // コンパイルパス作成
		compailPath = compailPath.generic_string(); // 区切り文字を / で統一する

		Microsoft::WRL::ComPtr<ID3DBlob> blob; // バイナリーデータ入れる

		if (!LoadCompiledShaderBlob(compailPath, blob.GetAddressOf())) { // コンパイルしているファイルを読み込む処理
			if (!JudgeCompileShader(kFilePath, hlslPath, blob)) { // 失敗したらコンパイル処理
				ErrorLog::MessageBoxOutput((hlslPath.string() + " : コンパイルに失敗しました").c_str());
				return false;
			}
		}

		if (!JudgeBinaryMenber(line, device, blob.Get()->GetBufferPointer(), blob.Get()->GetBufferSize())) { // シェーダーを作成する
			ErrorLog::MessageBoxOutput((hlslPath.string() + " : シェーダーの作成に失敗しました").c_str());
			return false;
		}
	}

	return true;
}


// ==================================================
//	// バイナリーデータを仕分けして、メンバー配列に代入する関数
// ==================================================
bool ShaderManager::JudgeBinaryMenber(const std::string shaderName, ID3D11Device* device, void* binary, size_t size)
{
	if (std::filesystem::path(shaderName).has_extension()) // 拡張子チェック
	{
		ErrorLog::Log("拡張子が付いています　確認してください");
		return false;
	}

	bool IsSuccess = true;

	// バイナリーデータをいれて、シェーダーを作成　配列に代入
	if (shaderName.rfind("VS_", 0) == 0) {
		auto vertex = std::make_unique<VertexShaderData>(shaderName, "main", "vs_5_0");  // 動的確保
		if (!vertex->CreateShader(device, binary, size)) {                                       // 初期化実行
			ErrorLog::Log(std::string("頂点シェーダー " + shaderName + " のクラスの初期化に失敗しました").c_str());
			IsSuccess = false;
		}
		m_Vertexs[shaderName] = std::move(vertex);                                       // メンバー配列に代入

#if defined(DEBUG) || defined(_DEBUG)
		m_Names.push_back(shaderName);                                               // 名前を保存しておく
#endif

	}
	else if (shaderName.rfind("PS_", 0) == 0) {
		auto pixel = std::make_unique<PixelShaderData>(shaderName, "main", "ps_5_0");      // 動的確保
		if (!pixel->CreateShader(device, binary, size)) {                                          // 初期化実行
			ErrorLog::Log(std::string("ピクセルシェーダ― " + shaderName + " のクラスの初期化に失敗しました").c_str());
			IsSuccess = false;
		}
		m_Pixels[shaderName] = std::move(pixel);                                           // メンバー配列に代入

#if defined(DEBUG) || defined(_DEBUG)
		m_Names.push_back(shaderName);                                               // 名前を保存しておく
#endif

	}
	else if (shaderName.rfind("CS_", 0) == 0) {
		auto compute = std::make_unique< ComputeShaderData>(shaderName, "main", "cs_5_0"); // 動的確保
		if (!compute->CreateShader(device, binary, size)) {                                        // 初期化実行
			ErrorLog::Log(std::string("コンピュートシェーダー " + shaderName + " のクラスの初期化に失敗しました").c_str());
			IsSuccess = false;
		}
		m_Computes[shaderName] = std::move(compute);                                       // メンバー配列に代入

#if defined(DEBUG) || defined(_DEBUG)
		m_Names.push_back(shaderName);                                               // 名前を保存しておく
#endif

	}
	else {
		ErrorLog::Log(std::string(shaderName + " : 先頭にシェーダーの種類が記載されていません").c_str()); // ログ出力
		IsSuccess = false;
	}

	if (!IsSuccess) {
		return false;
	}
	return true;
}


// ===============================================
// シェーダーをセットする関数
// ===============================================
void ShaderManager::BindVertexShaderSet(const std::string name, ID3D11DeviceContext* context)
{
	auto vs = m_Vertexs.find(name);

	if (vs != m_Vertexs.end()) { // あった場合
		context->VSSetShader(vs->second->GetShader(), nullptr, 0);
	}
	else{
		ErrorLog::Log(("頂点シェーダ " + name + " がありませんでした").c_str());
	}
}
void ShaderManager::BindPixelShaderSet(const std::string name, ID3D11DeviceContext* context)
{
	auto ps = m_Pixels.find(name);

	if (ps != m_Pixels.end()) { // あった場合
		context->PSSetShader(ps->second->GetShader(), nullptr, 0);
	}
	else {
		ErrorLog::Log(("ピクセルシェーダ― " + name + " がありませんでした").c_str());
	}
}
void ShaderManager::BindComputeShaderSet(const std::string name, ID3D11DeviceContext* context)
{
	auto cs = m_Computes.find(name);

	if (cs != m_Computes.end()) { // あった場合
		context->CSSetShader(cs->second->GetShader(), nullptr, 0);
	}
	else {
		ErrorLog::Log(("コンピュートシェーダ " + name + " がありませんでした").c_str());
	}
}


// ===============================================
// シェーダーを探して、返す関数
// ===============================================
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


// =================================================
// シェーダーをコンパイルする関数
// =================================================
bool OutputCompileShader(const std::filesystem::path kFilePath, const std::filesystem::path path, const std::string entryPoint, const std::string shaderTypeModel, ID3DBlob** blob)
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

	std::string filename = path.stem().string();                                   // 拡張子なしの名前を取得
	std::filesystem::path outputPath = kFilePath / (filename + kCompileExtension); // 出力パスを作成

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
	ofs.flush();                                                                                          // すぐに書き込んでもらう
	ofs.close();                                                                                          // ファイルを閉じる

	*blob = compileBlob.Detach();

	// 一応明示的に解放　
	errorBlob.Reset();
	compileBlob.Reset();

	return true;
}


// ============================================================================
// コンパイルして外部に書き出したファイルをBlobに取り込む関数
// ============================================================================
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
	if (!ifs) {
		ErrorLog::Log(std::string(csoPath.string() + " : バイナリーデータの読み込みに失敗しました").c_str());
		ifs.close();
		return false;
	}

	ifs.close(); // ファイルを閉じる

	return true;
}


// ==================================================================
// コンパイルするシェーダーの種類を判定する
// ==================================================================
bool JudgeCompileShader(const std::filesystem::path kFilePath, const std::filesystem::path filename, Microsoft::WRL::ComPtr<ID3DBlob>& blob)
{
	// ファイルの最初の名前でシェーダー判定
	if (filename.stem().string().rfind("VS_", 0) == 0) {
		if (!OutputCompileShader(kFilePath, filename, "main", "vs_5_0", blob.GetAddressOf())) // コンパイルして書き出す
		{
			ErrorLog::Log(std::string("頂点シェーダー " + filename.string() + " のコンパイル失敗").c_str());
			return false;
		}
	}
	else if (filename.stem().string().rfind("PS_", 0) == 0) {
		if (!OutputCompileShader(kFilePath, filename, "main", "ps_5_0", blob.GetAddressOf())) // コンパイルして書き出す
		{
			ErrorLog::Log(std::string("ピクセルシェーダー " + filename.string() + " のコンパイル失敗").c_str());
			return false;
		}
	}
	else if (filename.stem().string().rfind("CS_", 0) == 0) {
		if (!OutputCompileShader(kFilePath, filename, "main", "cs_5_0", blob.GetAddressOf())) // コンパイルして書き出す
		{
			ErrorLog::Log(std::string("コンピュートシェーダ " + filename.string() + " のコンパイル失敗").c_str());
			return false;
		}
	}
	else {
		ErrorLog::Log(std::string(filename.string() + " : 先頭にシェーダーの種類が記載されていません").c_str()); // ログ出力
		return  false;
	}

	return true;
}


// ============================================================
// リフレクション関数
// ============================================================
bool Reflect(void* blob, size_t blobSize, std::vector<ConstantBufferInfo>& outBuffers) {
	Microsoft::WRL::ComPtr<ID3D11ShaderReflection> reflector; // リフレクション
	HRESULT hr = D3DReflect(blob, blobSize, IID_PPV_ARGS(&reflector)); // バイナリーを解析
	if (ErrorLog::IsSuccessHRESULTWithOutputToConsole(hr, "リファレンス失敗")) { // 失敗したかの判定
		return false;
	}

	D3D11_SHADER_DESC shaderDesc = {}; 
	reflector->GetDesc(&shaderDesc);// 定数バッファの情報を取得

	for (int i = 0; i < (int)shaderDesc.ConstantBuffers; ++i) { // 定数バッファの数だけループする
		ID3D11ShaderReflectionConstantBuffer* cb = reflector->GetConstantBufferByIndex(i); // i番目の定数バッファ情報を取得

		D3D11_SHADER_BUFFER_DESC bufferDesc = {};
		cb->GetDesc(&bufferDesc); //名前やサイズを取得

		// バインドポイントを取得（bindSlot）
		int bindPoint = UINT_MAX;
		for (UINT b = 0; b < shaderDesc.BoundResources; ++b) {
			D3D11_SHADER_INPUT_BIND_DESC bindDesc = {};
			reflector->GetResourceBindingDesc(b, &bindDesc);

			if (bindDesc.Type == D3D_SIT_CBUFFER &&
				bindDesc.Name != nullptr &&
				bufferDesc.Name != nullptr &&
				std::string(bindDesc.Name) == bufferDesc.Name)
			{
				bindPoint = bindDesc.BindPoint; // バインドスロット情報を取得
				break;
			}
		}

		if (bindPoint != UINT_MAX && bufferDesc.Name != nullptr 
			&& bufferDesc.Name[0] != '\0') { // バインドスロットが見つからなかったとき、Nameに何も入っていないときは失敗
			ConstantBufferInfo info;
			info.name = bufferDesc.Name;
			info.bindSlot = bindPoint;
			info.size = bufferDesc.Size;
			outBuffers.push_back(info);
		}
		else{
			ErrorLog::Log("バインド番号もしくは名前が取得できませんでした");
			return false;
		}
	}

	return true;
}


#if defined(DEBUG) || defined(_DEBUG) // デバッグ時のみ有効にする関数

// 同じ階層にある.hlslを全て探して、コンパイルするかを確認する関数
// 最終更新日をコンパイルファイルと比較してコンパイルするかを決める
bool ShaderManager::DebugInit(ID3D11Device* device)
{
	std::filesystem::path currentFilePath = __FILE__;                       // このソースコードのパスを取得
	std::filesystem::path currentDirectory = currentFilePath.parent_path(); // パスから自分の階層だけを抜き取る
	bool IsSuccess = true; // 成否判定

	// .hlslファイルを探す
	for (const auto& entry : std::filesystem::directory_iterator(currentDirectory)) // 階層内のファイルを全て取得しています
	{
		if (!entry.is_regular_file() || entry.path().extension() != kShaderExtension) { continue; } // ファイルでなかったり、拡張子が違ったりすれば次のループへ

		std::filesystem::path hlslPath = entry.path();                                                            // .hlslのフルパスをを取得
		std::filesystem::path filename = hlslPath.filename();                                                     // .hlslのファイル名を取得
		std::filesystem::path csoPath = std::filesystem::path(kFilePath) / (filename.stem().wstring() + L".cso"); // .hlslの拡張子なしファイル名に.csoを付けてコンパイル名を取得

		Microsoft::WRL::ComPtr<ID3DBlob> blob; // バイナリーデータ入れる

		if (IsShaderUpdateCheck(hlslPath, csoPath)) { // 更新日時を調べる
			if (!JudgeCompileShader(kFilePath, filename, blob)) { // コンパイル処理
				ErrorLog::MessageBoxOutput((filename.string() + " : コンパイルに失敗しました").c_str());
				return false;
			}
		}
		else {	// .cso を読み込む
			if (!LoadCompiledShaderBlob(csoPath, blob.GetAddressOf())) { // コンパイルしているファイルを読み込む処理
				if (!JudgeCompileShader(kFilePath, filename, blob)) { // 失敗したらコンパイル処理
					ErrorLog::MessageBoxOutput((filename.string() + " : コンパイルに失敗しました").c_str());
					return false;
				}
			}
		}

		if (!JudgeBinaryMenber(filename.stem().string(), device, blob.Get()->GetBufferPointer(), blob.Get()->GetBufferSize())) { // バイナリーデータを渡して、シェイダーを作成する
			ErrorLog::MessageBoxOutput("シェイダーの初期化に失敗しました");
			return false;
		}

		blob.Reset(); // 一応、解放処理
	}

	if (!WriteLog()) {// 配列をログにして書き出す
		ErrorLog::Log("シェイーダーのログ書出しに失敗");
		return false;
	}

	return true;
}


// コンパイルファイルがない場合と,hlslが更新されていたらコンパイルされます
// 最終更新日を比較して、コンパイルが必要かをチェックしています
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