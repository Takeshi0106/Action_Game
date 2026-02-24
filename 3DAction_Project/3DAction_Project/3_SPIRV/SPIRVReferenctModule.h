#pragma once

// =========================================
// 【クラス概要】
// SPIR-Vのリフレクション情報を保持するクラス
// すべての PCOS,APIで使用することを想定しています
// 
// 今は、入力レイアウトと定数バッファの情報を保持することを想定しています
// 将来的には、テクスチャやサンプラーの情報を使用して、
// エディターでのリソースの自動割り当てなどに使用することも行えるように
// =========================================


// =========================================
// ヘッダー
// =========================================
// 文字列ヘッダー
#include "../UTF8_String.h"


// =========================================
// クラス
// =========================================
class SPIRVReferenctModule final
{
private:
	// -----------------------------------
	// バイナリーSPIRV-V 関連
	// ------------------------------------
	// SPIR-V保存先フォルダー名
	const String& kSPIRVFolderPath;
	// SPIR-Vの拡張子
	const String kSPIRVExtension = u8".spv";

	// -----------------------------------
	// リファレクション 構造体関連
	// ------------------------------------
	// SPIR-Vリフレクション情報の保存先パス
	const String& kSPIRVReflectionInfoFolderPath;

	// ------------------------------------
	// デバッグ用 リファレクションテキストファイル関連
	// ------------------------------------
	// SPiR-Vリフレクション情報のテキストファイル保存先フォルダーパス
	const String& kSPIRVReflectionInfoTextPath;
	// SPIR-Vリフレクション情報の拡張子
	const String kSPIRVReflectionInfoTextExtension = u8".txt";

	// -----------------------------------
	// 文字列書き出し用
	// -----------------------------------
	// シェーダー名
	const String kShaderName = u8"ShaderName : ";

	// 入力レイアウト
	const String kIL = u8"InputLayout : ";
	const String kILName = u8"ILName : ";
	const String kILIndex = u8"ILIndex : ";
	const String kILFormat = u8"ILFormat : ";

	// 定数バッファ
	const String kCBuffer = u8"ConstantBuffer : ";
	const String kCBName = u8"CBName : ";
	const String kRegisterNumber = u8"RegisterNumber : ";
	const String kCBSize = u8"CBSize : ";


	// -----------------------------------
	// メンバー関数
	// -----------------------------------
	// 文字列書き出し用関数
	bool WriteTextRefarenceInfo(const String& _shaderName);

public:
	// ----------------------------------------
	// コンストラクタ・デストラクタ
	// ----------------------------------------
	SPIRVReferenctModule(
		const String& _spirvPath,
		const String& _refPath,
		const String& _textPath)
		:kSPIRVFolderPath(_spirvPath),
		kSPIRVReflectionInfoFolderPath(_refPath),
		kSPIRVReflectionInfoTextPath(_textPath) {
	}

	~SPIRVReferenctModule() = default;

	// ----------------------------------------
	// SPIR-Vのリフレクション情報を書き出すクラス
	// ----------------------------------------
	bool WriteSPIRVReflectionInfo(
		const String& _shaderName);

	// ----------------------------------------
	// SPIR-Vリフレクション情報の外部ファイルを削除する関数
	// ----------------------------------------
	bool DeleteSPIRVReflectionInfo(const String& _shaderName);
};

