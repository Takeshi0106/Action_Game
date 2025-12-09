#pragma once

// =============================
// [クラス概要]
// ナイトクラス
// =============================


// ==============================
// ヘッダー
// ==============================
#include "ColliderObject.h"
#include <string>


// ==============================
// クラス
// ==============================
class Knight final : public ColliderObject
{
private:
	// --------------------------------
	// メンバー変数
	// --------------------------------
	// モデル名
	std::string m_ModelName = "Mech_FinnTheFrog";
	
	// シェーダー名
	std::string m_VSName = "VS_Object";
	std::string m_PSName = "PS_TextureModel";


	//--------------------------------
	// 非仮想関数
	// --------------------------------
	// 遅延初期化
	void DerivationInit() override final;
	// 更新
	void DerivationUpdate() override final;

public:
	// --------------------------------
	// 非仮想関数
	// --------------------------------
	// 描画
	void Draw() override final;
	// 後処理
	void Uninit() override final;
};

