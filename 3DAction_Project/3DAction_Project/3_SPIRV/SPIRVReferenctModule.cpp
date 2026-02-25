

#if defined(DEBUG) || defined(_DEBUG)

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
// バイナリーデータ
#include "../BinaryView.h"
// リファレンスヘッダー
extern "C"{
#include "spirv_reflect.h"
}


// =========================================
// SPIR-Vリファレンス書き出し関数
// =========================================
bool SPIRVReferenctModule::WriteSPIRVReflectionInfo(const String& _shaderName)
{
	// --------------------------------------------
	// バイナリーデータを取得
	// --------------------------------------------
	// SPIR-Vバイナリーパスを作成
	std::filesystem::path spirvPath = std::filesystem::path(kSPIRVFolderPath.GetU8String()) /
		(_shaderName + kSPIRVExtension).GetU8String();
	// 区切り文字を統一
	spirvPath = spirvPath.generic_string();


	// --------------------------------------------
	// SPIR-V 取得
	// --------------------------------------------
	// バイナリーデータ取得
	BinaryData binaryData;

	// SPIR-V を取得する
	if (!FileUtis::Binary::ReadBinaryFile(spirvPath.u8string(), binaryData)) {
		ErrorLog::OutputToConsole(u8"SPIR-Vファイルの読み込みに失敗しました。");
		return false;
	}

	// 読み込んだファイルがSPIR-Vかどうかを判定
	uint32_t magic = *reinterpret_cast<const uint32_t*>(binaryData.GetData());

	if (magic != 0x07230203) {
		ErrorLog::OutputToConsole(u8"SPIR-Vではありません");
		return false;
	}


	// --------------------------------------------
	// SPIR-V リフレクション
	// --------------------------------------------
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


	// --------------------------------------------
	// 自作リファレクション構造体に変換
	// --------------------------------------------
	SelfReflectionInfo selfRefInfo;
	// 名前を保存
	selfRefInfo.shaderName = _shaderName;

	// 自作リファレクション構造体に変換
	if (!m_SelfRefDataModule.ConvertSpvReflectToSelfRefData(
		module,
		selfRefInfo))
	{
		ErrorLog::OutputToConsole(u8"SPIR-Vリフレクション構造体の自作構造体への変換に失敗しました。");
		return false;
	}

	// バイナリーデータ書き出し
	WriteBinaryRefarenceInfo(selfRefInfo);

	// テキストファイル書き出し
	WriteTextRefarenceInfo(selfRefInfo);

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
// SPIR-Vリフレクション情報をバイナリーデータに書き出す関数
// =========================================
bool SPIRVReferenctModule::WriteBinaryRefarenceInfo(
	const SelfReflectionInfo& _refInfo)
{
	// ----------------------------------------------------
	// 入力リファレクションバイナリー書き出し
	// ----------------------------------------------------
	// 入力レイアウト書き出しパス作成
	std::filesystem::path savePath = 
		std::filesystem::path(kSPIRVReflectionInfoFolderPath.GetU8String()) /
		kSPIRVRefILPath.GetU8String() /
		(_refInfo.shaderName + kSPIRVReflectionInfoExtension).GetU8String();
	// 区切り文字を統一
	savePath = savePath.generic_string();

	// バイナリーデータ作成
	BinaryView ilBinaryView(_refInfo.ilInfos.data(),
		_refInfo.ilInfos.size() * sizeof(Self_ILInfo));

	// ファイルに書き出す
	if (!FileUtis::Binary::WriteBinaryFile(savePath.u8string(), ilBinaryView)) {
		ErrorLog::OutputToConsole(u8"入力レイアウトのバイナリーデータの書き出しに失敗しました。");
		return false;
	}

	// ----------------------------------------------------
	// 定数バッファリファレクションバイナリー書き出し
	// ----------------------------------------------------
	// 定数バッファ書き出しパス作成
	std::filesystem::path cbSavePath =
		std::filesystem::path(kSPIRVReflectionInfoFolderPath.GetU8String()) /
		kSPIRVRefCBPath.GetU8String() /
		(_refInfo.shaderName + kSPIRVReflectionInfoExtension).GetU8String();
	// 区切り文字を統一
	cbSavePath = cbSavePath.generic_string();

	// バイナリーデータ作成
	BinaryView cbBinaryView(_refInfo.cbInfos.data(),
		_refInfo.cbInfos.size() * sizeof(Self_CBInfo));

	// ファイルに書き出す
	if (!FileUtis::Binary::WriteBinaryFile(cbSavePath.u8string(), cbBinaryView)) {
		ErrorLog::OutputToConsole(u8"定数バッファのバイナリーデータの書き出しに失敗しました。");
		return false;
	}

	return true;
}


// =========================================
// SPIR-Vリフレクション情報を外部テキストファイルに書き出す関数
// =========================================
bool SPIRVReferenctModule::WriteTextRefarenceInfo(
	const SelfReflectionInfo& _refInfo)
{
	// ----------------------------------------------------
	// 外部テキストファイルに書き出す内容を作成
	// ----------------------------------------------------
	// SPIR-Vリフレクション情報をファイルに書き出す
	String reflectionInfo;

	// シェーダー名
	reflectionInfo += SaveUtils::MakeTypeInfo(kShaderName, _refInfo.shaderName, 0);
	// リファレクション内容を入れるための文字列
	String data;

	// 入力レイアウト
	for (size_t i = 0; i < _refInfo.ilInfos.size(); i++)
	{
		// 入力レイアウトの情報を取得
		Self_ILInfo ilInfo = _refInfo.ilInfos[i];

		// 入力変数の情報を文字列にする
		data +=
			// ブロックの内容
			SaveUtils::MakeTypeInfo(kILName, ilInfo.name, 2) +
			SaveUtils::MakeTypeInfo(kILIndex, String::to_u8string(ilInfo.index), 2) +
			SaveUtils::MakeTypeInfo(kILFormat, kVertexFormatString[static_cast<size_t>(ilInfo.format)], 2);

		data += String(u8"\n");
	}

	// 入力レイアウトがある場合
	if (_refInfo.ilInfos.size() > 0)
	{
		// ブロックにする
		reflectionInfo += SaveUtils::FormatBlock(
			kIL,
			_refInfo.ilInfos.size(),
			data,
			1);

		// 改行
		reflectionInfo += String(u8"\n");
	}

	// 文字列初期化
	data.Clear();

	// 定数バッファ
	for (size_t i = 0; i < _refInfo.cbInfos.size(); i++)
	{
		// 定数バッファ取得
		Self_CBInfo constant = _refInfo.cbInfos[i];

		data +=
			// ブロックの内容
			SaveUtils::MakeTypeInfo(kCBName, constant.name, 2) +
			SaveUtils::MakeTypeInfo(kRegisterNumber, String::to_u8string(constant.index), 2) +
			SaveUtils::MakeTypeInfo(kCBSize, String::to_u8string(constant.size), 2);

		data += String(u8"\n");
	}

	if (_refInfo.cbInfos.size() > 0)
	{
		reflectionInfo += SaveUtils::FormatBlock(
			// ブロック名
			kCBuffer,
			// 定数バッファの数をブロックの数として使用
			_refInfo.cbInfos.size(),
			data,
			1);
	}

	// ----------------------------------------------------
	// SPIR-Vリフレクション情報の保存先パスを作成
	// ----------------------------------------------------
	std::filesystem::path savePath = 
		std::filesystem::path(kSPIRVReflectionInfoFolderPath.GetU8String()) /
		kSpIRVRefTextPath.GetU8String() /
		(_refInfo.shaderName + kSPIRVReflectionInfoTextExtension).GetU8String();

	// 区切り文字を統一
	savePath = savePath.generic_string();


	// -----------------------------------------------------
	// ファイルに書き出す
	// -----------------------------------------------------
	if (!FileUtis::Text::WriteTextFile(savePath.u8string(), reflectionInfo)) {
		ErrorLog::OutputToConsole(u8"SPIR-Vリフレクション情報の書き出しに失敗しました。");
		return false;
	}


	return true;
}

#endif
