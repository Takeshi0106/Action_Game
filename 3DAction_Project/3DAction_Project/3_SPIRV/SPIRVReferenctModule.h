#pragma once

// =========================================
// 【クラス概要】
// SPIR-Vのリフレクションを自作リファレクションに変換して書き出すモジュール
// すべての PCOS,PCAPIで使用することを想定しています
// 
// 今は、入力レイアウトと定数バッファの情報を保持することを想定しています
// 将来的には、テクスチャやサンプラーの情報を使用して、
// エディターでのリソースの自動割り当てなどに使用することも行えるように
// デバッグでしか使えないようにしています
// =========================================


#if defined(DEBUG) || defined(_DEBUG)

// =========================================
// ヘッダー
// =========================================
// 文字列ヘッダー
#include "../UTF8_String.h"
// 自作リファレクション変換モジュール
#include "../SPIRV_SelfRefDataModule.h"


// =========================================
// 前方宣言
// =========================================
// SPIR-Vリフレクション構造体の前方宣言
struct SpvReflectShaderModule;


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
	// 自作リファレクション変換モジュール
	SPIRV_SelfRefDataModule m_SelfRefDataModule;

	// SPIR-Vリフレクション構造体情報の保存先パス
	const String& kSPIRVReflectionInfoFolderPath;
	// SPIR-Vリフレクション構造体情報の拡張子
	const String kSPIRVReflectionInfoExtension = u8".bin";

	// SPIR-Vリファレクション の入力レイアウト保存先パス
	const String kSPIRVRefILPath = u8"InputLayout";
	// SPIR-Vリファレクション の定数バッファ保存先パス
	const String kSPIRVRefCBPath = u8"ConstantBuffer";

	// ------------------------------------
	// デバッグ用 リファレクションテキストファイル関連
	// ------------------------------------
	// SPIR-Vリフレクションテキスト情報の保存先パス
	const String kSpIRVRefTextPath = u8"Text";
	// SPIR-Vリフレクション情報の拡張子
	const String kSPIRVReflectionInfoTextExtension = u8".txt";

	// -----------------------------------
	// 文字列書き出し用
	// -----------------------------------
	// シェーダー名
	const String kShaderName = u8"ShaderName";

	// 入力レイアウト
	const String kIL = u8"InputLayout";
	const String kILName = u8"ILName";
	const String kILIndex = u8"ILIndex";
	const String kILFormat = u8"ILFormat";

	// 定数バッファ
	const String kCBuffer = u8"ConstantBuffer";
	const String kCBName = u8"CBName";
	const String kRegisterNumber = u8"RegisterNumber";
	const String kCBSize = u8"CBSize";


	// -----------------------------------
	// メンバー関数
	// -----------------------------------
	// リファレクション構造体書き出し用関数
	bool WriteBinaryRefarenceInfo(
		const SelfReflectionInfo& _refInfo);

	// 文字列書き出し用関数
	bool WriteTextRefarenceInfo(
		const SelfReflectionInfo& _refInfo);

public:
	// ----------------------------------------
	// コンストラクタ・デストラクタ
	// ----------------------------------------
	SPIRVReferenctModule(
		const String& _spirvPath,
		const String& _refPath)
		:kSPIRVFolderPath(_spirvPath),
		kSPIRVReflectionInfoFolderPath(_refPath){
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

#endif
