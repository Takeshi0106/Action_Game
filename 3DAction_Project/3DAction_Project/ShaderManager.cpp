
// ===========================================
// ヘッダー
// ===========================================

// 必須ヘッダー
#include "ShaderManager.h"         // 自分のヘッダー
#include "ShaderData.h"            // シェイダーデータクラス
#include "ConstantBufferManager.h" // CBのマネージャー

// 情報を保存する配列
#include <vector> // 読み出したファイルを保存する配列

// DirectX用スマートポインター
#include <wrl/client.h>  // DirectX用のスマートポインター

// シェイダーコンパイル用ヘッダー
#include "ShaderCompil.h"

// シェーダーリファレクション用ヘッダー
#include "ShaderReflection.h"

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
// コンパイルするシェイダーの種類を判定してコンパイル関数に渡す
bool JudgeCompileShader(const std::filesystem::path kFilePath, const std::filesystem::path filename, Microsoft::WRL::ComPtr<ID3DBlob>& blob);

#if defined(DEBUG) || defined(_DEBUG)
// デバッグ用関数

// .hlslの最終更新日を確認して、更新されているかをチェックする
bool IsShaderUpdateCheck(const std::filesystem::path& shaderPath, const std::filesystem::path& binaryPath);

#endif


// =================================================
// 初期化
// =================================================
bool ShaderManager::Init(ID3D11Device* device, ConstantBufferManager& CBManager)
{
#if defined(DEBUG) || defined(_DEBUG)

	// デバッグ用初期化
	// 同じ階層にあるシェーダーを調べて、使用する
	// リファレンスしてシェーダーとCBManagerに渡す
	if (!DebugInit(device, CBManager)) {
		ErrorLog::MessageBoxOutput("Debug : シェーダーマネージャーの初期化失敗");
		return false;
	}

#else

	// リリース用初期化
	// 外部ファイルから、使用するシェーダー情報を取得して、
	// シェーダーとCBManagerに渡す
	if (!ReleaseInit(device, CBManager)) {
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
	// 各シェーダーを解放
	m_Vertexs.clear();  // 中身を削除 unique_ptrのためDeleteされる
	m_Pixels.clear();   // 中身を削除 unique_ptrのためDeleteされる
	m_Computes.clear(); // 中身を削除 unique_ptrのためDeleteされる
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
	}
	else if (shaderName.rfind("PS_", 0) == 0) {
		auto pixel = std::make_unique<PixelShaderData>(shaderName, "main", "ps_5_0");      // 動的確保
		if (!pixel->CreateShader(device, binary, size)) {                                          // 初期化実行
			ErrorLog::Log(std::string("ピクセルシェーダ― " + shaderName + " のクラスの初期化に失敗しました").c_str());
			IsSuccess = false;
		}
		m_Pixels[shaderName] = std::move(pixel);                                           // メンバー配列に代入
	}
	else if (shaderName.rfind("CS_", 0) == 0) {
		auto compute = std::make_unique< ComputeShaderData>(shaderName, "main", "cs_5_0"); // 動的確保
		if (!compute->CreateShader(device, binary, size)) {                                        // 初期化実行
			ErrorLog::Log(std::string("コンピュートシェーダー " + shaderName + " のクラスの初期化に失敗しました").c_str());
			IsSuccess = false;
		}
		m_Computes[shaderName] = std::move(compute); // メンバー配列に代入
	}
	else {
		ErrorLog::Log(std::string(shaderName + " : 先頭にシェーダーの種類が記載されていません").c_str()); // ログ出力
		IsSuccess = false;
	}

	DebugSetName(shaderName.c_str());  // 名前を保存しておくデバッグ用

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





#if defined(DEBUG) || defined(_DEBUG) 
// デバッグ時のみ有効にする関数

// ====================================================================
// デバッグ用の初期化関数
// 同じ階層にある.hlslを全て探して、コンパイルするかを確認する関数
// 最終更新日をコンパイルファイルと比較してコンパイルするかを決める
// ====================================================================
bool ShaderManager::DebugInit(ID3D11Device* device, ConstantBufferManager& CBManager)
{
	// このソースコードのパスを取得
	std::filesystem::path currentFilePath = __FILE__;
	// パスから自分の階層だけを抜き取る
	std::filesystem::path currentDirectory = currentFilePath.parent_path();

	// 全てのシェーダーのリファレンスした情報を入れる
	std::vector<ShaderInfo> allShaderInfo;

	// .hlslファイルの数を見積もる
	size_t shaderFileCount = std::count_if(
		std::filesystem::directory_iterator(currentDirectory), // ファイルの場所を指定
		std::filesystem::directory_iterator{},                 // どこまで探すかを指定　今はそのパスだけを指定しています
		[](const auto& entry) {
			return entry.is_regular_file() && entry.path().extension() == kShaderExtension; // 条件判定をラムダ式にして渡しています
		});

	// 見積もった数でメモリサイズを決める
	allShaderInfo.resize(shaderFileCount);
	// allShaderInfoのインデックス
	int index = 0;

	// .hlslファイルを探す処理
	for (const auto& entry : std::filesystem::directory_iterator(currentDirectory))
	{
		// 階層内の全てのファイルをを所得して、ファイルでなかったり、拡張子が違ったりすれば次のループへ
		if (!entry.is_regular_file() || entry.path().extension() != kShaderExtension) { continue; }

		// 見積もった数より多いかのチェック
		if (shaderFileCount < index) {
			ErrorLog::Log(".hlslファイルの数が一致しません");
			return false;
		}

		// .hlslのフルパスをを取得
		std::filesystem::path hlslPath = entry.path();
		// .hlslのファイル名を取得
		std::filesystem::path filename = hlslPath.filename();
		// コンパイルファイルパスを取得
		std::filesystem::path csoPath = std::filesystem::path(kCSOFilePath) / (filename.stem().wstring() + L".cso");

		// バイナリーデータ入れる
		Microsoft::WRL::ComPtr<ID3DBlob> blob;


		// コンパイルする必要があるかを調べる
		if (IsShaderUpdateCheck(hlslPath, csoPath))
		{
			// コンパイルする処理
			if (!JudgeCompileShader(kCSOFilePath, filename, blob)) {
				ErrorLog::MessageBoxOutput((filename.string() + " : コンパイルに失敗しました").c_str());
				return false;
			}
		}
		else
		{
			// コンパイルファイルを読み込む
			if (!LoadCompiledShader(csoPath, blob.GetAddressOf()))
			{
				// 失敗したらコンパイル処理
				if (!JudgeCompileShader(kCSOFilePath, filename, blob)) {
					ErrorLog::MessageBoxOutput((filename.string() + " : コンパイルに失敗しました").c_str());
					return false;
				}
			}
		}

		// リフレクション情報を入れる配列
		std::vector<ConstantBufferInfo> conInfo;
		std::vector<InputLayoutInfo> ilInfo;

		// リフレクション情報を代入
		if (!Reflect(blob.Get()->GetBufferPointer(), blob.Get()->GetBufferSize(), conInfo, ilInfo)) {
			ErrorLog::MessageBoxOutput("リフレクションした情報が得られませんでした");
			return false;
		}

		// 頂点シェーダー以外の時入力レイアウトを削除する
		if (filename.string().find("VS") != 0)
		{
			ilInfo.clear();
		}

		// シェイダーを作成する
		if (!JudgeBinaryMenber(filename.stem().string(), device, blob.Get()->GetBufferPointer(), blob.Get()->GetBufferSize())) {
			ErrorLog::MessageBoxOutput("シェイダーの初期化に失敗しました");
			return false;
		}

		// 配列に代入
		allShaderInfo[index].shaderName = filename.stem().string();
		allShaderInfo[index].CBInfo = std::move(conInfo);
		allShaderInfo[index].ILInfo = std::move(ilInfo);

		// 一応、解放処理
		blob.Reset();
		
		// インデックスを更新
		index++;
	}

	// 出力関数
	ShaderInfoOutput(kShaderInfoPath, allShaderInfo);

	// 使用した情報の名前をログにして書き出す
	if (!WriteLog()) {
		ErrorLog::Log("シェイーダーのログ書出しに失敗");
		return false;
	}

	// デバッグ用


	return true;
}


// ===========================================================================
// コンパイルするかチェックする関数
// コンパイルファイルがない場合と,hlslが更新されていたらコンパイルされます
// 最終更新日を比較して、コンパイルが必要かをチェックしています
// ===========================================================================
bool IsShaderUpdateCheck(const std::filesystem::path& shaderPath, const std::filesystem::path& binaryPath)
{
	// ファイルにアクセス時に例外が発生する可能性があるため、使用しています
	try 
	{
		// .csoファイルが存在しなければ更新する
		if (!std::filesystem::exists(binaryPath)) {
			return true;
		}

		// 両ファイルの更新日時取得
		auto shaTime = std::filesystem::last_write_time(shaderPath);
		auto binTime = std::filesystem::last_write_time(binaryPath);

		// .hlslのほうが新しければ更新
		if (shaTime > binTime) 
		{
			// デバッグ用にログ出力
			ErrorLog::Log((shaderPath.string() + " が新しく更新されています").c_str());
			return true;
		}

	}
	catch (const std::filesystem::filesystem_error& e) 
	{
		// ファイルアクセスに失敗した場合もコンパイル必須にする
		ErrorLog::Log((std::string("IsShaderUpdateCheck ファイルアクセスエラー: ") + e.what()).c_str());
		return true;
	}

	// デバッグ用にログ出力
	ErrorLog::Log(std::string(shaderPath.stem().string() + " 更新する必要がないためコンパイルファイルを取得します  相対パス : " + binaryPath.string()).c_str());

	return false;
}


#else
// ==================================================
// リリース時の初期化関数
// 
// 外部ファイルを取得して、コンパイルされているかを確認
// 必要な情報をシェーダーとCBManagerに渡す
// ==================================================
bool ShaderManager::ReleaseInit(ID3D11Device* device, ConstantBufferManager& CBManager)
{
	// 中身を入れる	配列
	std::vector<ShaderInfo> allShaderInfo;

	// リフレクション外部情報を取得
	if (!ShaderInfoInput(kShaderInfoPath, allShaderInfo)) {
		ErrorLog::MessageBoxOutput("リフレクション情報を取得失敗");
		return false;
	}

	// シェーダーを作成
	for (int i = 0; i < allShaderInfo.size(); i++)
	{
		// シェーダーパスを作成
		std::filesystem::path hlslPath = std::filesystem::path(kHlslFailePath) / (allShaderInfo[i].shaderName + kShaderExtension);
		hlslPath = hlslPath.generic_string(); // 区切り文字を / で統一する　

		// コンパイルパス作成
		std::filesystem::path compailPath = std::filesystem::path(kCSOFilePath) / (allShaderInfo[i].shaderName + kCompileExtension);
		compailPath = compailPath.generic_string(); // 区切り文字を / で統一する

		Microsoft::WRL::ComPtr<ID3DBlob> blob; // バイナリーデータ入れる

		// コンパイルしているファイルを読み込む処理
		if (!LoadCompiledShader(compailPath, blob.GetAddressOf()))
		{
			// 失敗したらコンパイル処理
			if (!JudgeCompileShader(kCSOFilePath, hlslPath, blob)) {
				ErrorLog::MessageBoxOutput((hlslPath.string() + " : コンパイルに失敗しました").c_str());
				return false;
			}
		}

		// シェーダーを作成する
		if (!JudgeBinaryMenber(allShaderInfo[i].shaderName, device, blob.Get()->GetBufferPointer(), blob.Get()->GetBufferSize())) {
			ErrorLog::MessageBoxOutput((hlslPath.string() + " : シェーダーの作成に失敗しました").c_str());
			return false;
		}

		// 定数バッファを作成
		for (int j = 0; j < allShaderInfo[i].info.size(); j++)
		{
			CBManager.CreateConstantBuffer(allShaderInfo[i].info[j].name, allShaderInfo[i].info[j].size,
				allShaderInfo[i].info[j].registerNumber, device);
		}
	}

	return true;
}


#endif