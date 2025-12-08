#pragma once

// =============================
// [クラス概要]
// ナイトクラス
// =============================


// ==============================
// ヘッダー
// ==============================
#include "BaseObject.h"
#include <string>


#if defined(DEBUG) || defined(_DEBUG)
// ＢＯＸ描画
#include "Box.h"
#endif


// ==============================
// クラス
// ==============================
class Knight final : public BaseObject
{
private:
	// モデル名
	std::string m_ModelName = "Mech_FinnTheFrog";
	
	// シェーダー名
	std::string m_VSName = "VS_Object";
	std::string m_PSName = "PS_TextureModel";

	// BOX描画
	BOX m_DebugBox;

	// 遅延初期化
	void LateInit() override final;
	// デバッグ描画
	void DebugDrawBox();

public:
	// 基本関数
	void Update() override final;
	void Draw() override final;
	void Uninit() override final;
};

