
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
// バイナリーデータ
#include "../BinaryData.h"
// エラーログ出力関数
#include "../ReportMessage.h"
// リファレンスヘッダー
extern "C"{
#include "spirv_reflect.h"
}


// =========================================
// SPIR-Vリファレンス書き出し関数
// =========================================
bool SPIRVReferenctModule::WriteSPIRVReflectionInfo(const String& _shaderName)
{
	WriteTextRefarenceInfo(_shaderName);

	return true;
}


// =========================================
// SPIR-Vリフレクション情報の削除関数
// =========================================
bool SPIRVReferenctModule::DeleteSPIRVReflectionInfo(const String& _shaderName)
{
	// SPIR-Vリフレクション情報の保存先パスを作成
	std::filesystem::path savePath = std::filesystem::path(kSPIRVReflectionInfoFolderPath.GetU8String()) /
		(_shaderName + kSPIRVReflectionInfoTextExtension).GetU8String();

	// 区切り文字を統一
	savePath = savePath.generic_string();

	// ファイルが存在する場合は削除
	if (std::filesystem::exists(savePath))
	{
		// エラーコードを取得
		std::error_code ec;

		if (!std::filesystem::remove(savePath, ec))
		{
			ErrorLog::OutputToConsole(u8"SPIR-Vリフレクション情報の削除に失敗しました: " + savePath.u8string() + u8" エラー: " +
				String::to_u8string(ec.value()));
			return false;
		}
	}

	return true;
}


// =========================================
// SPIR-Vリフレクション情報を外部テキストファイルに書き出す関数
// =========================================
bool SPIRVReferenctModule::WriteTextRefarenceInfo(const String& _shaderName)
{
	// SPIR-Vリフレクション情報の保存先パスを作成
	std::filesystem::path savePath = std::filesystem::path(kSPIRVReflectionInfoTextPath.GetU8String()) /
		(_shaderName + kSPIRVReflectionInfoTextExtension).GetU8String();

	// 区切り文字を統一
	savePath = savePath.generic_string();

	// SPIR-V 保存先パスを作成
	std::filesystem::path spirvPath = std::filesystem::path(kSPIRVFolderPath.GetU8String()) /
		(_shaderName + kSPIRVExtension).GetU8String();

	// 区切り文字を統一
	savePath = savePath.generic_string();

	// バイナリーデータ取得
	BinaryData binaryData;

	// SPIR-V を取得する
	if (!FileUtis::Binary::ReadBinaryFile(spirvPath.u8string(), binaryData)) {
		ErrorLog::OutputToConsole(u8"SPIR-Vファイルの読み込みに失敗しました。");
		return false;
	}

	uint32_t magic = *reinterpret_cast<const uint32_t*>(binaryData.GetData());

	if (magic != 0x07230203) {
		ErrorLog::OutputToConsole(u8"SPIR-Vではありません（DXILの可能性）");
		return false;
	}

	// SPIR-Vリフレクション情報を取得
	SpvReflectShaderModule module;

	// SPIR-Vリフレクション情報の取得
	SpvReflectResult result = spvReflectCreateShaderModule(
		binaryData.GetSize(),
		binaryData.GetData(),
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
	// 入力レイアウトの内容を入れるための文字列
	String data;

	// 入力レイアウト
	for (uint32_t i = 0; i < module.input_variable_count; i++)
	{
		// 入力変数の情報を取得
		SpvReflectInterfaceVariable* inputVar = module.input_variables[i];

		// 入力変数の情報を文字列にする
		data +=
			// ブロックの内容
			SaveUtils::MakeTypeInfo(kILName, inputVar->name ? String(reinterpret_cast<const char8_t*>(inputVar->name)) : String(u8"Unnamed"), 2) +
			SaveUtils::MakeTypeInfo(kILIndex, String::to_u8string(inputVar->location), 2) +
			SaveUtils::MakeTypeInfo(kILFormat, String::to_u8string(inputVar->format), 2);

		data += String(u8"\n");
	}

	// ブロックにする
	reflectionInfo += SaveUtils::FormatBlock(
		kIL,
		module.input_variable_count,
		data,
		1);

	// 改行
	reflectionInfo += String(u8"\n");

	// 文字列初期化
	data.Clear();

	// 定数バッファ
	for (uint32_t i = 0; i < module.descriptor_binding_count; i++)
	{
		// ディスクリプタバインディングの情報を取得
		SpvReflectDescriptorBinding* descriptorBinding = &module.descriptor_bindings[i];

		// 定数バッファの情報を文字列にする
		if (descriptorBinding->descriptor_type == SPV_REFLECT_DESCRIPTOR_TYPE_UNIFORM_BUFFER)
		{
			data +=
				// ブロックの内容
				SaveUtils::MakeTypeInfo(kCBName, descriptorBinding->name ? String(reinterpret_cast<const char8_t*>(descriptorBinding->name)) : String(u8"Unnamed"), 2) +
				SaveUtils::MakeTypeInfo(kRegisterNumber, String::to_u8string(descriptorBinding->binding), 2) +
				SaveUtils::MakeTypeInfo(kCBSize, String::to_u8string(descriptorBinding->block.size), 2);

			data += String(u8"\n");
		}
	}

	reflectionInfo += SaveUtils::FormatBlock(
		// ブロック名
		kCBuffer,
		// 定数バッファの数をブロックの数として使用
		module.descriptor_binding_count,
		data,
		1);

	reflectionInfo = SaveUtils::FormatAnonymousBlock(reflectionInfo, 0);

	// ファイルに書き出す
	if (!FileUtis::Text::WriteTextFile(savePath.u8string(), reflectionInfo)) {
		ErrorLog::OutputToConsole(u8"SPIR-Vリフレクション情報の書き出しに失敗しました。");
		return false;
	}
	return true;
}
