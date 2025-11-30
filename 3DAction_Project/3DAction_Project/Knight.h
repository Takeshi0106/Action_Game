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

	// 遅延初期化
	void LateInit() override final;

public:
	// 基本関数
	void Update() override final;
	void Draw() override final;
	void Uninit() override final;
};

