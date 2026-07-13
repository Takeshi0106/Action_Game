#pragma once

// =========================================
// 【クラス概要】
// SPIR-Vのパスを渡して、自作のリフレクション情報を返すクラス
// =========================================
// ヘッダー
#include "UTF8_String.h"
#include "RefarencesData.h"

// =========================================
// クラス
// =========================================
class SPIRVReferenceModule final
{
private:
	// メンバー関数
	String assetPath;

	// リファレクション構造体書き出し用関数
	bool WriteBinaryRefarenceInfo(const SelfReflectionInfo& _refInfo);

public:
	// コンストラクタ・デストラクタ
#if defined(DEBUG) || defined(_DEBUG)
	SPIRVReferenceModule(const String& _assetPath) : assetPath(_assetPath) {}
#else
	SPIRVReferenceModule(const String& _assetPath) {}
#endif
	~SPIRVReferenceModule() = default;

	// SPIR-Vのリフレクション情報を取得するクラス
	SelfReflectionInfo SPIRVReflection(const String& _shaderFilePath);
};
