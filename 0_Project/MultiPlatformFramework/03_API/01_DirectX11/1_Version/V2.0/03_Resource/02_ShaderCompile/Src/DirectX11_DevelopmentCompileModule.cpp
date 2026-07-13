// ヘッダー
#include "DirectX11_DevelopmentCompileModule.h"
#include <filesystem>
#include "ReportMessage.h"

// シェーダーコンパイル
bool DX11_DevelopmentCompileModule::ShaderCompile(const DX11_CompileMode _mode)
{
	// HLSLフォルダーパスを取得
	std::filesystem::path currentDirectory = kHlslPath.GetU8String();
	// HLSLフォルダー内の.hlslファイル数を見積もる
	size_t shaderFileCount = std::count_if(
		std::filesystem::directory_iterator(currentDirectory),
		std::filesystem::directory_iterator{},
		[this](const auto& entry) {
			return entry.is_regular_file() && entry.path().extension() == (std::filesystem::path)kHlslExtension.GetU8String();});
	
	// .hlslファイルを探して、コンパイル
	int hlslCount = 0;
	for (const auto& entry : std::filesystem::directory_iterator(currentDirectory))
	{
		// ファイル出なかったり、拡張子が違ったりすれば次のループへ
		if (!entry.is_regular_file() || entry.path().extension() != (std::filesystem::path)kHlslExtension.GetU8String()) { continue; }

		// 念のためチェック
		if ((int)shaderFileCount < hlslCount) {
			ErrorLog::OutputToConsole(u8".hlslファイルの数が一致しません");
		}

		// .hlslのパスをを取得
		std::filesystem::path hlslPath = entry.path();
		hlslPath.make_preferred();

		// シェーダーのコンパイル
		if (!ShaderCompile(hlslPath.stem().u8string(), _mode)) 
		{
			DeleteCompileShader(hlslPath.stem().u8string());
			ErrorLog::OutputToConsole(u8"シェーダーのコンパイルに失敗しました: " + hlslPath.u8string());
			return false;
		}
	}
	return true;
}

// シェーダーコンパイル関数
bool DX11_DevelopmentCompileModule::ShaderCompile(
	const String& _hlslName, 
	const DX11_CompileMode _mode)
{
	// DXBCコンパイル
	BinaryData blobData = m_ShaderCompileModule.ShaderCompil(_hlslName, _mode);
	if (blobData.IsEmpty()) {
		ErrorLog::OutputToConsole(u8"DXBCコンパイルに失敗しました。");
		return false;
	}
	// SPIR-Vコンパイル
	if (!m_SPIRVCompileModule.SPIRVCompile(_hlslName)) {
		ErrorLog::OutputToConsole(u8"SPIR-Vコンパイルに失敗しました。");
		return false;
	}
	// SPIR-Vリファレンス書き出し
	SelfReflectionInfo selfReflectionInfo = m_SPIRVReferenceModule.SPIRVReflection(_hlslName);
	if (selfReflectionInfo.shaderName.IsEmpty()) {
		ErrorLog::OutputToConsole(u8"SPIR-Vリファレンスの書き出しに失敗しました。");
		return false;
	}

	return true;
}

// 外部ファイル削除関数
bool DX11_DevelopmentCompileModule::DeleteCompileShader(const String& _hlslName)
{
	// SPIR-Vコンパイルファイル削除
	if (!m_SPIRVCompileModule.DeleteCompileShader(_hlslName)) {
		ErrorLog::OutputToConsole(u8"SPIR-Vコンパイルファイルの削除に失敗しました。");	
		return false;
	}

	return true;
}

/*
// コンパイルするかのチェック関数
bool IsCompileCheck(
	const std::filesystem::path& _hlslPath,
	const std::filesystem::path& _compilePath)
{
	// バイナリファイルが存在するかチェック
	if (!std::filesystem::exists(_compilePath)) {
		return true;
	}
	// 最終更新日時を取得
	auto shaderLastWriteTime = std::filesystem::last_write_time(_hlslPath);
	auto binaryLastWriteTime = std::filesystem::last_write_time(_compilePath);

	// シェーダーファイルの方が新しい場合は更新が必要
	return shaderLastWriteTime > binaryLastWriteTime;
}
*/
