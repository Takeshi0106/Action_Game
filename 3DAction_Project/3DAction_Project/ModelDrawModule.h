#pragma once

// =============================
// クラス概要
// モデルを描画するモジュール
// 
// ＊注意
// 位置などの定数情報は更新しないとそのまま使用されます。
// =============================


// =============================
// ヘッダー
// =============================
// 抽象化描画マネージャー
#include "BaseDrawManager.h"
// 文字列ヘッダー
#include "UTF8_String.h"


// =============================
// クラス
// =============================
class ModelDrawModule final
{
private:
	// ----------------------------------
	// メンバー変数
	// ----------------------------------
	// 描画マネージャー
	BaseDrawManager* m_Draw = nullptr;


public:
	// ---------------------------------
	// コンストラクタ・デストラクタ
	// ---------------------------------
	ModelDrawModule() = default;
	virtual ~ModelDrawModule() = default;


	// ----------------------------------
	// 基本関数
	// ----------------------------------
	// 初期化
	void Init(BaseDrawManager& _draw, 
		const String& _modelName, 
		const String& _fileName = u8"");
	
	// 描画
	void Draw(
		const String& _modelName,
		const String& _vsShaderName,
		const String& _psShaderName);

	// 終了処理
	void Uninit();


	// ===================================
	// セッター
	// ===================================
	void CreateModel(
		const String& _modelName, 
		const String& _fileName);
};

