// ヘッダー
#include "SPIRVReferenceModule.h"
#include <filesystem>
#include "FileUtils.h"
#include "SaveLoadUtils.h"
#include "BinaryData.h"
#include "ReportMessage.h"
extern "C"{
#include "spirv_reflect.h"
}
// SPIRVリフレクションの補助関数
#include "SPIRVReflectionUtils.h"

// SPIR-Vリファレンス情報を取得する関数
SelfReflectionInfo SPIRVReferenceModule::SPIRVReflection(const String& _shaderFilePath)
{
	// SPIR-Vバイナリーパスを作成
	std::filesystem::path spirvPath = std::filesystem::path(_shaderFilePath.GetU8String());
	spirvPath.make_preferred();

	// SPIR-V 取得
	BinaryData binaryData;
	if (!FileUtis::Binary::ReadBinaryFile(spirvPath.u8string(), binaryData)) {
		ErrorLog::OutputToConsole(u8"SPIR-Vファイルの読み込みに失敗しました。");
		return SelfReflectionInfo();
	}

	// 読み込んだファイルがSPIR-Vかどうかチェック
	uint32_t magic = *reinterpret_cast<const uint32_t*>(binaryData.GetData());
	if (magic != 0x07230203) {
		ErrorLog::OutputToConsole(u8"SPIR-Vではありません");
		return SelfReflectionInfo();
	}

	// SPIR-V リフレクション
	SpvReflectShaderModule module;
	SpvReflectResult result = spvReflectCreateShaderModule(
		binaryData.GetSize(),
		binaryData.GetData(),
		&module);
	if (result != SPV_REFLECT_RESULT_SUCCESS) {
		ErrorLog::OutputToConsole(u8"SPIR-Vリフレクション情報の取得に失敗しました。");
		return SelfReflectionInfo();
	}

	// 自作リファレクション構造体に変換
	SelfReflectionInfo selfRefInfo;
	selfRefInfo.shaderName = String(spirvPath.filename().stem().u8string());
	if (!SPIRV::ReflectionUtils::ConvertSpvReflectToSelfRefData(
		module,
		selfRefInfo))
	{
		ErrorLog::OutputToConsole(u8"SPIR-Vリフレクション構造体の自作構造体への変換に失敗しました。");
		return SelfReflectionInfo();
	}

	// SPIR-Vリフレクション情報の解放
	spvReflectDestroyShaderModule(&module);
	return selfRefInfo;
}
