#pragma once

// ===============================================
// クラス概要
// モデルローダーモジュール
// モデルをロードしてモデルデータを返す
// 今は.obj形式のみ対応
// ===============================================


// ===============================================
// ヘッダー
// ===============================================
// モデルデータヘッダー
#include "ModelData.h"
// 文字列ヘッダー
#include "UTF8_String.h"


// ===============================================
// クラス
// ===============================================
class ModelLoadeModule
{
private:
	// --------------------------------
	// メンバー変数
	// --------------------------------
	// モデル読み込みパス
	const String& kModelPath;
	// 対応拡張子
	const String kObjExtension = u8".obj";


public:
	// --------------------------------
	// コンストラクタ・デストラクタ
	// --------------------------------
	ModelLoadeModule(const String& _config) :
		kModelPath(_config)
	{}
	~ModelLoadeModule() = default;


	// --------------------------------
	// メンバー関数
	// --------------------------------
	// モデル読み込み内部関数
	ModelData ModelLoad(const String& modelPath, const String& modelFile = u8"");

	// モデル読込パスを返す
	const String& GetModelLoadPath() const { return kModelPath; }
};

