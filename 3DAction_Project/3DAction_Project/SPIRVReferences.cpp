
// =========================================
// ヘッダー
// =========================================
// 必須ヘッダー
#include "SPIRVReferences.h"
// ファイルシステム
#include <filesystem>
// ログ出力
#include "ReportMessage.h"
// ファイル書き出しヘッダー
#include "FileUtils.h"
// セーブロード補助関数
#include "SaveLoadUtils.h"
// リファレンスヘッダー
extern "C"{
#include "3_SPIRV/spirv_reflect.h"
}


// =========================================
// SPIR-Vリファレンス書き出し関数
// =========================================
bool SPIRVReferences::WriteSPIRVReflectionInfo(const String& _shaderName, const BinaryView& _binaly)
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

	return true;
}
