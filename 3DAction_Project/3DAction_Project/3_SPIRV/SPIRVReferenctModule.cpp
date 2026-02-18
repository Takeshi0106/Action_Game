
// =========================================
// ヘッダー
// =========================================
// 必須ヘッダー
#include "SPIRVReferenctModule.h"
// ファイルシステム
#include <filesystem>
// ログ出力
#include "../ReportMessage.h"
// ファイル書き出しヘッダー
#include "../FileUtils.h"
// セーブロード補助関数
#include "../SaveLoadUtils.h"
// エラーログ出力関数
#include "../ReportMessage.h"
// リファレンスヘッダー
extern "C"{
#include "spirv_reflect.h"
}


// =========================================
// SPIR-Vリファレンス書き出し関数
// =========================================
bool SPIRVReferenctModule::WriteSPIRVReflectionInfo(const String& _shaderName, const BinaryView& _binaly)
{
	// SPIR-Vリフレクション情報の保存先パスを作成
	std::filesystem::path savePath = std::filesystem::path(kSPIRVReflectionInfoFolderPath.GetU8String()) /
		(_shaderName + kSPIRVReflectionInfoExtension).GetU8String();

	// SPIR-Vリフレクション情報を取得
	SpvReflectShaderModule module;

	// SPIR-Vリフレクション情報の取得
	SpvReflectResult result = spvReflectCreateShaderModule(
		_binaly.GetSize(),
		_binaly.GetData(),
		&module);

	// エラーチェック
	if (result != SPV_REFLECT_RESULT_SUCCESS) {
		ErrorLog::OutputToConsole(u8"SPIR-Vリフレクション情報の取得に失敗しました。");
		return false;
	}

	// SPIR-Vリフレクション情報をファイルに書き出す
	String reflectionInfo;

	// シェーダー名
	reflectionInfo += SaveUtils::MakeTypeInfo(kShaderName, _shaderName, 0);

	// 入力レイアウト
	for (uint32_t i = 0; i < module.input_variable_count; i++)
	{
		// 入力変数の情報を取得
		SpvReflectInterfaceVariable* inputVar = module.input_variables[i];

		// 入力変数の情報を文字列にする
		reflectionInfo += SaveUtils::FormatBlock(
			// ブロック名
			kIL,
			// 入力変数の数をブロックの数として使用
			module.input_variable_count,
			// ブロックの内容
			SaveUtils::MakeTypeInfo(kILName, inputVar->name ? String(reinterpret_cast<const char8_t*>(inputVar->name)) : String(u8"Unnamed"), 1) +
			SaveUtils::MakeTypeInfo(kILIndex, String::to_u8string(inputVar->location), 1) +
			SaveUtils::MakeTypeInfo(kILFormat, String::to_u8string(inputVar->format), 1),
			// 空白の数
			0);
	}

	// 定数バッファ
	for (uint32_t i = 0; i < module.descriptor_binding_count; i++)
	{
		// ディスクリプタバインディングの情報を取得
		SpvReflectDescriptorBinding* descriptorBinding = &module.descriptor_bindings[i];

		// 定数バッファの情報を文字列にする
		if (descriptorBinding->descriptor_type == SPV_REFLECT_DESCRIPTOR_TYPE_UNIFORM_BUFFER)
		{
			reflectionInfo += SaveUtils::FormatBlock(
				// ブロック名
				kCBuffer,
				// 定数バッファの数をブロックの数として使用
				module.descriptor_binding_count,
				// ブロックの内容
				SaveUtils::MakeTypeInfo(kCBName, descriptorBinding->name ? String(reinterpret_cast<const char8_t*>(descriptorBinding->name)) : String(u8"Unnamed"), 1) +
				SaveUtils::MakeTypeInfo(kRegisterNumber, String::to_u8string(descriptorBinding->binding), 1) +
				SaveUtils::MakeTypeInfo(kCBSize, String::to_u8string(descriptorBinding->block.size), 1),
				// 空白の数
				0);
		}
	}

	// ファイルに書き出す
	if (!FileUtis::Text::WriteTextFile(savePath.u8string(), reflectionInfo)) {
		ErrorLog::OutputToConsole(u8"SPIR-Vリフレクション情報の書き出しに失敗しました。");
		return false;
	}

	return true;
}
