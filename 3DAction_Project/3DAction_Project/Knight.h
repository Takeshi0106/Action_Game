#pragma once

// =============================
// [クラス概要]
// ナイトクラス
// =============================


// ==============================
// ヘッダー
// ==============================
#include "ColliderObject.h"
#include "UTF8_String.h"


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
	Hashed_String m_ModelName = Hashed_String(u8"Mech_FinnTheFrog");
	
	// シェーダー名
	Hashed_String m_VSName = Hashed_String(u8"VS_Object");
	Hashed_String m_PSName = Hashed_String(u8"PS_TextureModel");


	//--------------------------------
	// 非仮想関数
	// --------------------------------
	// 遅延初期化
	void DerivationInit() override final;

public:
	// --------------------------------
	// 非仮想関数
	// --------------------------------
	// 更新
	void Update() override final;
	// 描画
	void Draw() override final;
	// 後処理
	void Uninit() override final;
};

