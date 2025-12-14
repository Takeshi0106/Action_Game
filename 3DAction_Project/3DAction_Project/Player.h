#pragma once

// ===================================
// 【クラス概要】
// プレイヤークラス
// ===================================


// ===================================
// ヘッダー
// ===================================
#include "ColliderObject.h"
#include <string>


// ===================================
// クラス
// ===================================
class Player final : public ColliderObject
{
private:
	// --------------------------------
	// メンバー変数
	// --------------------------------
	// モデル名
	const std::string m_ModelName = "Mech_FinnTheFrog";

	// シェーダー名
	const std::string m_VSName = "VS_Object";
	const std::string m_PSName = "PS_TextureModel";

	// --------------------------------
	// 非仮想関数
	// --------------------------------
	// 遅延初期化
	void DerivationInit() override final;

public:
	// --------------------------------
	// コンストラクタ・デストラクタ
	// --------------------------------
	Player() = default;
	~Player() override final = default;


	// ---------------------------------
	// 非仮想関数
	// ---------------------------------
	// 更新
	void Update() override final;
	// 描画
	void Draw() override final;
	// 後処理
	void Uninit() override final;
};

