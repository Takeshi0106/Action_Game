
// =============================================
// ヘッダー
// =============================================
// 必須ヘッダー
#include "DirectX11_DevelopmentCompileModule.h"
// ファイルシステム
#include <filesystem>
// ログ出力
#include "../../../../ReportMessage.h"


// =============================================
// シェーダーコンパイル
// =============================================
bool DX11_DevelopmentCompileModule::ShaderCompile(const DX11_CompileMode _mode)
{
	// HLSLフォルダーパスを取得
	std::filesystem::path currentDirectory = kHlslPath.GetU8String();

	// HLSLフォルダー内の.hlslファイル数を見積もる
	size_t shaderFileCount = std::count_if(
		std::filesystem::directory_iterator(currentDirectory),
		std::filesystem::directory_iterator{},
		[this](const auto& entry) {
			return entry.is_regular_file() && entry.path().extension() == (std::filesystem::path)kHlslExtension.GetU8String();
		});

	// インデックス
	int hlslCount = 0;

	// .hlslファイルを探す処理
	for (const auto& entry : std::filesystem::directory_iterator(currentDirectory))
	{
		// 階層内の全てのファイルをを所得して、ファイルでなかったり、拡張子が違ったりすれば次のループへ
		if (!entry.is_regular_file() || entry.path().extension() != (std::filesystem::path)kHlslExtension.GetU8String()) { continue; }

		// 念のためチェック
		if ((int)shaderFileCount < hlslCount) {
			ErrorLog::OutputToConsole(u8".hlslファイルの数が一致しません");
		}

		// .hlslのパスをを取得
		std::filesystem::path hlslPath = entry.path();

		// 区切り文字を統一
		hlslPath = hlslPath.generic_string();

		// シェーダーのコンパイル
		if (!ShaderCompile(hlslPath.stem().u8string(), _mode)) 
		{
			// シェーダー削除
			DeleteCompileShader(hlslPath.stem().u8string());
			ErrorLog::OutputToConsole(u8"シェーダーのコンパイルに失敗しました: " + hlslPath.u8string());
			return false;
		}
	}

	return true;
}


// =============================================
// シェーダーコンパイル関数
// =============================================
bool DX11_DevelopmentCompileModule::ShaderCompile(
	const String& _hlslName, 
	const DX11_CompileMode _mode)
{
	// DXBCコンパイル
	if (!m_ShaderCompileModule.ShaderCompil(_hlslName, _mode)) {
		ErrorLog::OutputToConsole(u8"DXBCコンパイルに失敗しました。");
		return false;
	}

	// SPIR-Vコンパイル
	if (!m_SPIRVCompileModule.SPIRVCompile(_hlslName)) {
		ErrorLog::OutputToConsole(u8"SPIR-Vコンパイルに失敗しました。");
		return false;
	}

	// SPIR-Vリファレンス書き出し
	if (!m_SPIRVReferenceModule.WriteSPIRVReflectionInfo(_hlslName)) {
		ErrorLog::OutputToConsole(u8"SPIR-Vリファレンスの書き出しに失敗しました。");
		return false;
	}

	return true;
}


// =============================================
// 外部ファイル削除関数
// =============================================
bool DX11_DevelopmentCompileModule::DeleteCompileShader(const String& _hlslName)
{
	// DXBCコンパイルファイル削除
	if (!m_ShaderCompileModule.DeleteCompileShader(_hlslName)) {
		ErrorLog::OutputToConsole(u8"DXBCコンパイルファイルの削除に失敗しました。");
		return false;
	}

	// SPIR-Vコンパイルファイル削除
	if (!m_SPIRVCompileModule.DeleteCompileShader(_hlslName)) {
		ErrorLog::OutputToConsole(u8"SPIR-Vコンパイルファイルの削除に失敗しました。");	
		return false;
	}

	// SPIR-Vリファレンスファイル削除
	if (!m_SPIRVReferenceModule.DeleteSPIRVReflectionInfo(_hlslName)) {
		ErrorLog::OutputToConsole(u8"SPIR-Vリファレンスファイルの削除に失敗しました。");
		return false;
	}

	return true;
}
