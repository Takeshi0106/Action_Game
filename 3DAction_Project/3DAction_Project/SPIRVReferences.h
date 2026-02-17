#pragma once

// =========================================
// 【クラス概要】
// SPIR-Vのリフレクション情報を保持するクラス
// すべての PCOS,APIで使用することを想定しています
// =========================================


// =========================================
// ヘッダー
// =========================================
// 文字列ヘッダー
#include "UTF8_String.h"
// バイナリーデータヘッダー
#include "BinaryView.h"


// =========================================
// クラス
// =========================================
class SPIRVReferences final
{
private:
	// SPIR-Vリフレクション情報の保存先パス
	const String& kSPIRVReflectionInfoFolderPath;
	// SPIR-Vリフレクション情報の拡張子
	const String kSPIRVReflectionInfoExtension = u8".txt";

	// -----------------------------------
	// 書き出し用
	// -----------------------------------
	// シェーダー名
	const String kShaderName = u8"ShaderName : ";

	// 入力レイアウト
	const String kILName = u8"ILName : ";
	const String kILIndex = u8"ILIndex : ";
	const String kILFormat = u8"ILFormat : ";

	// 定数バッファ
	const String kCBName = u8"CBName : ";
	const String kRegisterNumber = u8"RegisterNumber : ";
	const String kCBSize = u8"CBSize : ";


public:
	// ----------------------------------------
	// コンストラクタ・デストラクタ
	// ----------------------------------------
	SPIRVReferences(const String& _path) 
		: kSPIRVReflectionInfoFolderPath(_path) {
	}
	~SPIRVReferences() = default;

	// ----------------------------------------
	// SPIR-Vのリフレクション情報を書き出すクラス
	// ----------------------------------------
	bool WriteSPIRVReflectionInfo(
		const String& _shaderName,
		const BinaryView& _binaly);
};

