
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


// ================================================
// グローバル変数
// ================================================
namespace {

	// リファレンス情報書き込み、読込み用
	// シェーダー関連
	const std::string kShaderCount = "ShaderCount :"; // シェーダーの数
	const std::string kShaderName = "ShaderName :";   // シェーダーの名前

	// 入力レイアウト関連
	const std::string kInputLayoutCount         = "InputLayoutCount :";         // 入力レイアウトの数
	const std::string kInputLayoutSemanticName  = "InputLayoutSemanticName :";  // セマンティック名
	const std::string kInputLayoutSemanticIndex = "InputLayoutSemanticIndex :"; // セマンティックインデックス
	const std::string kInputLayoutInputSlot     = "InputLayoutInputSlot :";     // 入力スロット番号
	const std::string kInputLayoutFormat        = "InputLayoutFormat :";        // フォーマット

	// 定数バッファ関連
	const std::string kCBufferCount = "CBufferCount :";     // 定数バッファの数
	const std::string kCBufferName = "CBufferName :";       // 定数バッファの名前
	const std::string kRegisterNumber = "RegisterNumber :"; // レジスタ番号
	const std::string kSize = "Size :";                     // バイト数 (16の倍数単位)

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
	// 今の行を入れる
	size_t pos = 0;

	// 最初の行（シェーダーの数）を取得
	size_t nextPos = dataView.find('\n', pos);
	if (nextPos == std::string_view::npos) {
		ErrorLog::OutputToMessageBox("シェーダー情報が取得できませんでした");
		return false;
	}

	// １行を取り出す
	std::string_view line = dataView.substr(pos, nextPos - pos);

	// kShaderCount の長さ分スキップ
	std::string_view info = line.substr(strlen(kShaderCount.c_str()));

	// 例外が発生する可能性がある
	try
	{
		// キャスト
		int shaderCount = std::stoi(std::string(info));

		// サイズを取得する
		loadShaderInfo.resize(shaderCount);
	}
	catch (...)
	{
		ErrorLog::OutputToConsole("シェーダーの数を取得できませんでした");
		return false;
	}

	// 次の位置を設定
	pos = nextPos + 1;

	int shaderIndex = -1;  // シェーダーの添え字
	int cBufferIndex = -1; // 定数バッファの添え字

	// 行の数だけループ
	while (true)
	{
		// 1行分のサイズを入れる
		nextPos = dataView.find('\n', pos);

		// 1行を取り出す
		std::string_view line =
			(nextPos == std::string_view::npos)
			// 改行が見つからなかったら今の位置から最後までを入れる
			? dataView.substr(pos)
			// 今の位置から次の位置までを入れる
			: dataView.substr(pos, nextPos - pos);

		// 文字列内に空白かタブがあった時削除する
		while (!line.empty() && (line.front() == ' ' || line.front() == '\t'))
		{
			line.remove_prefix(1);
		}

		// ---------------------------------------------------------------------
		// 文字列の型を戻して、配列に代入する処理
		// ---------------------------------------------------------------------
		// シェーダーの名前を取得
		if (line.size() >= kShaderName.size() &&
			line.substr(0, kShaderName.size()) == kShaderName)
		{
			// kShaderName の長さ分スキップ
			info = line.substr(strlen(kShaderName.c_str()));

			shaderIndex++; // シェーダーの添え字を更新

			// シェーダーの名前を取得
			loadShaderInfo[shaderIndex].shaderName = info;
		}

		// 定数バッファの数を取得
		else if (line.size() >= kCBufferCount.size() &&
			line.substr(0, kCBufferCount.size()) == kCBufferCount)
		{
			// kCBufferCount の長さ分スキップ
			info = line.substr(strlen(kCBufferCount.c_str()));

			// 定数バッファ更新
			cBufferIndex = -1;

			try
			{
				// キャスト
				int cBufferCount = std::stoi(std::string(info));
				// ベクトルのサイズを取得
				loadShaderInfo[shaderIndex].CBInfo.resize(cBufferCount);
			}
			catch (...)
			{
				ErrorLog::OutputToConsole("定数バッファの数を取得できませんでした");
				return false;
			}
		}

		// 定数バッファの名前を取得
		else if (line.size() >= kCBufferName.size() &&
			line.substr(0, kCBufferName.size()) == kCBufferName)
		{
			// kCBufferName の長さ分スキップ
			info = line.substr(strlen(kCBufferName.c_str()));

			cBufferIndex++; // 定数バッファの添え字を更新

			// 定数バッファの名前を取得
			loadShaderInfo[shaderIndex].CBInfo[cBufferIndex].SetName(std::string(info));
		}

		// レジスタ番号を取得
		else if (line.size() >= kRegisterNumber.size() &&
			line.substr(0, kRegisterNumber.size()) == kRegisterNumber)
		{
			// kRegisterNumber の長さ分スキップ
			info = line.substr(strlen(kRegisterNumber.c_str()));

			try
			{
				// キャスト
				int registerNumber = std::stoi(std::string(info));
				// レジスタの取得
				loadShaderInfo[shaderIndex].CBInfo[cBufferIndex].SetRegisterNumber(registerNumber);
			}
			catch (...)
			{
				ErrorLog::OutputToConsole("レジスタ番号を取得できませんでした");
				return false;
			}

		}
		else if (line.size() >= kSize.size() &&
			line.substr(0, kSize.size()) == kSize)
		{
			// kRegisterNumber の長さ分スキップ
			info = line.substr(strlen(kSize.c_str()));

			try
			{
				// キャスト
				size_t size = static_cast<size_t>(std::stoul(std::string(info)));
				// レジスタの取得
				loadShaderInfo[shaderIndex].CBInfo[cBufferIndex].SetSize(size);
			}
			catch (...)
			{
				ErrorLog::OutputToConsole("定数バッファのサイズを取得できませんでした");
				return false;
			}
		}

		// 改行が見つからなければ、ループを抜ける
		if (nextPos == std::string_view::npos) { break; }

		// 次の位置を設定
		pos = nextPos + 1;
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

	// ファイルを開ける
	std::ofstream ofs(kShaderInfoPath, std::ios::binary | std::ios::out);
	if (!ofs.is_open()) {
		ErrorLog::OutputToConsole("使用したシェイダーログ : ファイルのオープンに失敗しました");
		return false;
	}

	// シェーダーの数を記載
	ofs << kShaderCount << shaderInfo.size() << "\n";
	// 見やすいように改行
	ofs << "\n";

	// リフレクション情報を書き出し
	for (const auto& shader : shaderInfo)
	{
		// シェーダーの名前
		ofs << kShaderName << shader.shaderName << "\n";

		// 入力レイアウトの数
		ofs << kInputLayoutCount << shader.ILInfo.size() << "\n";
		

		// 入力情報の数だけループ
		for (const auto& iLayout : shader.ILInfo)
		{
			// 入力レイアウトの名前
			ofs << "  " << kInputLayoutSemanticName << iLayout.GetSemanticName() << "\n";
			// 入力レイアウトのインデックス
			ofs << "    " << kInputLayoutSemanticIndex << iLayout.GetSemanticIndex() << "\n";
			// 入力レイアウトのスロット番号
			ofs << "    " << kInputLayoutInputSlot << iLayout.GetInputSlot() << "\n";
			// 入力レイアウトのフォーマット
			ofs << "    " << kInputLayoutFormat << iLayout.GetFormat() << "\n";
		}


		// 定数バッファの数
		ofs << kCBufferCount << shader.CBInfo.size() << "\n";

		// 定数バッファの数だけループする
		for (const auto& cbuffer : shader.CBInfo)
		{
			// 空白を分けて見やすいようにしています

			// 定数バッファの名前
			ofs << "  " << kCBufferName << cbuffer.GetName() << "\n";
			// 定数バッファのレジスタ番号
			ofs << "    " << kRegisterNumber << cbuffer.GetRegisterNumber() << "\n";
			// 定数バッファの大きさ
			ofs << "    " << kSize << cbuffer.GetSize() << "\n";
		}

		// シェーダーごとに改行
		ofs << "\n";
	}

	// ファイルを閉じる
	ofs.close();

	return true;
}

#endif